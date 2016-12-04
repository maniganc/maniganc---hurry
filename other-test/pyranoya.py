#!/usr/bin/python

import subprocess
import argparse
import re
import shlex

import resource
import time

# args parser
args_parser = argparse.ArgumentParser(description='simple unit tester')
args_parser.add_argument('--cmd', type=str, nargs=1,
                         help='shell command to test')
args_parser.add_argument('--test', type=str, nargs=1,
                         help='test file to test with')
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

            if not error_flag:
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
                if output != list(zip(*lines_to_check_against)[1]):
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
                if error_flag:
                    print final_header + term.color("expected success, but failed",
                                                    'bold', 'red')
                else:
                    print final_header + term.color("succeeded as expected",
                                                    'bold', 'green')
                    test_counter_success += 1
            else:
                if not error_flag:
                    print final_header + term.color("expected failure, got success",
                                                    'bold', 'red')
                else:
                    print final_header + term.color("failed as expected",
                                                    'bold', 'green')
                    test_counter_success += 1

            # cleanup
            lines_to_feed = []
            lines_to_check_against = []

    if test_counter == 0:
        print header + term.color("%s no tests executed" % (fp), 'underline', 'yellow')

    elif test_counter == test_counter_success:
        print header + term.color("%s all %s tests succeeded" % (fp, test_counter),
                                  'underline', 'green')

    elif test_counter != test_counter_success:
        print header + term.color("%s %s out of %s tests failed"
                                  % (fp, test_counter - test_counter_success, test_counter),
                                  'underline', 'red')

    test_file.close()
