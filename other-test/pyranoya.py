#!/usr/bin/python

import subprocess
import argparse
import re
import shlex

import resource
import time

# args parser
args_parser = argparse.ArgumentParser(description='simple unit tester')
args_parser.add_argument('--test', type=str, nargs='+',
                         help='test file to test with')
args_parser.add_argument('--check-valgrind', type=None,
                         help='check memory leaks')
args = args_parser.parse_args()

# test parsers
test_description_re = re.compile('^;; *description: +(.+)$')
test_command_re     = re.compile('^;; *command: +(.+)$')

# feed
test_feed_re        = re.compile('^;; *feed')
test_pause_feed_re  = re.compile('^;; *pause feed')

# check
test_check_re       = re.compile('^;; *check')
test_pause_check_re = re.compile('^;; *pause check')

# run params
test_command_timeout_re = re.compile(';; *command timeout +(.+)$')
test_command_expect_re  = re.compile(';; *command expect +(.+)$')

# run
test_run_command_re = re.compile('^;; *run command')

test_valgrind_errors_re = re.compile('^.*: (.+) errors.*$')


# colors for terminal
class term:
    styles = { 'normal'    : 0,
               'bold'      : 1,
               'italic'    : 3,
               'underline' : 4,
               'blink'     : 5}

    colors =  { 'black'   : 0,
                'red'     : 1,
                'green'   : 2,
                'yellow'  : 3,
                'blue'    : 4,
                'magenta' : 5,
                'cyan'    : 6,
                'white'   : 7}

    @staticmethod
    def color(string, style=None, fg=None, bg=None):
        format = []
        if style: format.append(str(term.styles[style]))
        if fg :   format.append(str(30+term.colors[fg]))
        if bg :   format.append(str(40+term.colors[bg]))
        format = ';'.join(format)
        return '\x1b[%sm%s\x1b[0m' % (format, string)


master_test_counter = 0
master_test_counter_success = 0

master_line_to_feed_counter = 0
master_line_to_check_against_counter = 0

if args.check_valgrind == "1":
    valgrind_check = True
else:
    valgrind_check = False

for test_filepath in args.test:
    fp = test_filepath
    test_file = open(fp, 'rb')

    description = ""
    command = ""

    feed_lines_flag = False
    lines_to_feed = []

    check_lines_flag = False
    lines_to_check_against = []

    run_time_limit   = 0 # in seconds
    command_expect_success = True

    test_counter = 0;
    test_counter_success = 0;

    for line_idx, line in enumerate(test_file.readlines()):
        line_number = line_idx + 1

        ########################################
        # get description
        match = test_description_re.match(line)
        if match:
            description = match.groups()[0]
            # print term.color("%s:%s %s" % (fp, line_number, description),
            #                  'bold', 'green')

        # get command
        match = test_command_re.match(line)
        if match:
            command = match.groups()[0]

        ########################################
        # pause check
        match = test_pause_check_re.match(line)
        if match:
            check_lines_flag = False

        if check_lines_flag == True:
            lines_to_check_against.append((line_number, line))

        # start check stdout lines
        match = test_check_re.match(line)
        if match:
            check_lines_flag = True
            feed_lines_flag = False  # also stop feeding stdout

        ########################################
        # pause feed
        match = test_pause_feed_re.match(line)
        if match:
            feed_lines_flag = False

        # feed lines to process
        if feed_lines_flag == True:
            lines_to_feed.append((line_number, line))

        # start feed
        match = test_feed_re.match(line)
        if match:
            feed_lines_flag = True

        ########################################
        match = test_command_timeout_re.match(line)
        if match:
            run_time_limit = float(match.groups()[0])

        match = test_command_expect_re.match(line)
        if match:
            expected_issue = match.groups()[0]
            if expected_issue == 'success':
                command_expect_success = True
            elif expected_issue == 'failure':
                command_expect_success = False
            else:
                raise ValueError('Unknown command expected issue: %s in %s:%s'
                                 % (expected_issue, fp, line_number))

        ########################################
        # run command
        match = test_run_command_re.match(line)
        if match:
            # start command with args
            command_args = shlex.split(command)
            command_popen = subprocess.Popen(command_args, shell=False,
                                             bufsize=1,
                                             stdin=subprocess.PIPE,
                                             stdout=subprocess.PIPE,
                                             stderr=subprocess.PIPE)

            # general error flag
            error_flag = False

            # feed stdout with test code
            for l_number, l in lines_to_feed:
                command_popen.stdin.write(l)

            # send EOF
            command_popen.stdin.close()

            time_begin = time.time() # time in seconds
            while command_popen.poll() == None and not error_flag:
                curr_time = time.time()

                # check timeout
                if run_time_limit != 0 and (curr_time - time_begin > run_time_limit):
                    # print error only if success was expected
                    if command_expect_success:
                        print term.color("%s:%s %s - error - timeout "
                                         % (fp, line_number, description),
                                         'bold', 'red')

                    command_popen.terminate()
                    error_flag = True

            # check return code
            if command_popen.returncode != 0 and not error_flag:
                if command_expect_success:
                    print term.color("%s:%s %s - error - return code is %s "
                                     % (fp, line_number, description,
                                        command_popen.returncode),
                                     'bold', 'red')

                error_flag = True

            if 1:#not error_flag:
                # check error messages
                errors = command_popen.stderr.readlines()
                if errors != []:
                    if command_expect_success:
                        print term.color("%s:%s %s - error - stderr message "
                                         % (fp, line_number, description),
                                         'bold', 'red')
                        for e in errors:
                            print term.color(e.replace('\n', ''), 'normal', 'red')

                    error_flag = True

                # check output
                output = command_popen.stdout.readlines()
                output_mispatch = 0
                if lines_to_check_against == []:
                    if output != []:
                        output_mispatch = 1

                else:
                    if output != list(zip(*lines_to_check_against)[1]):
                        output_mispatch = 1

                if output_mispatch == 1:
                    if command_expect_success:
                        print term.color("%s:%s %s - error - unexpected output "
                                         % (fp, line_number, description),
                                         'bold', 'red')
                        for c in zip(output, lines_to_check_against):
                            got = c[0]
                            exp = c[1][1]
                            exp_line = c[1][0]
                            if c[0] != c[1][1]:
                                print term.color("expected: (line %s)" % (exp_line)
                                                 , 'normal', 'yellow')
                                print term.color(exp.replace('\n', ''),
                                                 'normal', 'yellow')
                                print term.color('got:',
                                                 'normal', 'red')
                                print term.color(got.replace('\n', ''),
                                                 'normal', 'red')

                    error_flag = True

            # valgrind check, memory leaks only
            error_valgrind_flag = False
            if valgrind_check and ((error_flag == False and command_expect_success) or
                                   (error_flag == True and not command_expect_success)) :
                vg_cmd = "valgrind --leak-check=yes --leak-check=full " + command
                vg_args = shlex.split(vg_cmd)
                vg_ps = subprocess.Popen(vg_args, bufsize=1,
                                         stdin=subprocess.PIPE,
                                         stdout=subprocess.PIPE,
                                         stderr=subprocess.PIPE)
                # feed stdout with test code
                for l_number, l in lines_to_feed:
                    vg_ps.stdin.write(l)

                # send EOF
                vg_ps.stdin.close()

                errors = vg_ps.stderr.readlines()
                match = test_valgrind_errors_re.match(errors[-1])
                errors_num = int(match.groups()[0])
                if errors_num == 0:
                    error_valgrind_flag = False

                else:
                    print term.color("valgrind failed %s errors"
                                     % (errors_num),
                                     'bold', 'red')
                    for l in lines_to_feed:
                        print l;

                    for e in errors:
                        print e.replace('\n', '');

                    error_valgrind_flag = True

            # refresh number of tests made so far for this file
            test_counter += 1

            # display test result
            header = term.color("%s:%s [%s] "
                                % (fp,
                                   line_number,
                                   test_counter),
                                'normal','magenta')
            descr = term.color('%s' % (description),
                               'bold')
            final_header = header + descr + ' '
            if command_expect_success:
                if error_flag or error_valgrind_flag:
                    print final_header + term.color("expected success, but failed",
                                                    'bold', 'red')
                else:
                    print final_header + term.color("passed as expected",
                                                    'bold', 'green')
                    test_counter_success += 1
            else:
                if (not error_flag) or error_valgrind_flag:
                    print final_header + term.color("expected failure, got success",
                                                    'bold', 'red')
                else:
                    print final_header + term.color("failed as expected",
                                                    'bold', 'green')
                    test_counter_success += 1




            # save some stats before cleanup
            master_line_to_feed_counter += len(lines_to_feed)
            master_line_to_check_against_counter += len(lines_to_check_against)

            # cleanup
            lines_to_feed = []
            lines_to_check_against = []

    if test_counter == 0:
        print term.color("%s no tests executed" % (fp), 'underline', 'yellow')

    elif test_counter == test_counter_success:
        print term.color("%s all %s tests passed" % (fp, test_counter),
                         'underline', 'green')

    elif test_counter != test_counter_success:
        print term.color("%s %s out of %s tests failed"
                         % (fp, test_counter - test_counter_success, test_counter),
                         'underline', 'red')

    # global stats
    master_test_counter += test_counter
    master_test_counter_success += test_counter_success

    test_file.close()

header = term.color("pyranoya ", 'bold')

if master_test_counter == 0:
    print header + term.color("no tests executed", 'underline', 'yellow')

else:
    print header + term.color("send %s lines, and checked %s lines"
                              % (master_line_to_feed_counter,
                                 master_line_to_check_against_counter),
                              'underline', 'blue')

    if master_test_counter == master_test_counter_success:
        print header + term.color("all %s tests passed" % (master_test_counter),
                                  'underline', 'green')

    else:
        print header + term.color("%s out of %s tests failed"
                                  % (master_test_counter - master_test_counter_success,
                                     master_test_counter),
                                  'underline', 'red')
