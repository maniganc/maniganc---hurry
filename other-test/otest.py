
import subprocess
import argparse
import re
import shlex

from Queue import Queue, Empty
from subprocess import PIPE, Popen
from threading  import Thread

# args parser
args_parser = argparse.ArgumentParser(description='simple unit tester')
args_parser.add_argument('--cmd', type=str, nargs=1,
                         help='shell command to test')
args_parser.add_argument('--test', type=str, nargs=1,
                         help='test file to test with')
args = args_parser.parse_args()
print args

# test parsers
test_description_re = re.compile('^;; *description: +(.+)$')
test_command_re     = re.compile('^;; *command: +(.+)$')

test_start_command_re = re.compile('^;; *start command')
test_stop_command_re  = re.compile('^;; *stop command')

test_feed_re       = re.compile('^;; *feed')
test_pause_feed_re = re.compile('^;; *pause feed')
test_stop_feed_re  = re.compile('^;; *stop feed')

test_check_re      = re.compile('^;; *check')
test_against_re    = re.compile('^;; *against')
test_stop_check_re = re.compile('^;; *stop check')

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


# http://stackoverflow.com/questions/375427/non-blocking-read-on-a-subprocess-pipe-in-python
def enqueue_output(out, queue):
    print '[[go]]'
    for line in iter(out.readline, ''):
        queue.put(line)

    print '[[end]]'
    out.close()


# http://stackoverflow.com/questions/375427/non-blocking-read-on-a-subprocess-pipe-in-python
def process_output(myprocess, queue): #output-consuming thread
    nextline = None
    buf = ''
    while True:
        #--- extract line using read(1)
        out = myprocess.stdout.read(1)
        if out == '' and myprocess.poll() != None: break
        if out != '':
            buf += out
            if out == '\n':
                nextline = buf
                buf = ''
        if not nextline: continue
        line = nextline
        nextline = None
        print '[newline %s]' % (line)
        queue.put(line)

for test_filepath in args.test:
    fp = test_filepath
    test_file = open(fp, 'rb')

    description = ""
    command = ""

    command_launched = False
    command_popen = None
    readline_queue = None
    thread = None

    send_line_to_process = False
    check_line_from_process = False

    for line_idx, line in enumerate(test_file.readlines()):
        line_number = line_idx + 1
        print line.replace('\n', '')

        # print description
        match = test_description_re.match(line)
        if match:
            description = match.groups()[0]
            print term.color("%s:%s %s" % (fp, line_number, description),
                             'bold', 'green')

        # get command
        match = test_command_re.match(line)
        if match:
            command = match.groups()[0]
            print term.color("%s:%s %s" % (fp, line_number, command),
                             'normal', 'blue')


        # start command
        match = test_start_command_re.match(line)
        if match:
            command_args = shlex.split(command)
            command_popen = subprocess.Popen(command_args, shell=True,
                                             bufsize=1,
                                             stdin=subprocess.PIPE,
                                             stdout=subprocess.PIPE)
            readline_queue = Queue()
            thread = Thread(target=process_output, args=(command_popen,
                                                         readline_queue))
            thread.daemon = True
            thread.start()
            command_launched = True

        # stop command
        match = test_stop_command_re.match(line)
        if match:
            command_popen.kill()
            command_launched = False


        # pause feed
        match = test_pause_feed_re.match(line)
        if match:
            send_line_to_process = False

        # stop feed
        match = test_stop_feed_re.match(line)
        if match:
            send_line_to_process = False
            command_popen.stdin.close()

        # feed lines to process
        if send_line_to_process:
            print '[writing %s]' % (line.replace('\n', ''))
            command_popen.stdin.write(line)
            command_popen.stdin.flush()

        # start feed
        match = test_feed_re.match(line)
        if match:
            send_line_to_process = True


        # stop check
        match = test_stop_check_re.match(line)
        if match:
            check_line_from_process = False

        # start check stdout lines
        match = test_check_re.match(line)
        if match:
            from time import sleep
            sleep(0.1)
            print '[clean]'
            while not readline_queue.empty():
                s = readline_queue.get()
                print '[removed %s]' % (s)

        # check lines
        if check_line_from_process:
            line_out = readline_queue.get(timeout=1)
            print '[got %s]' % (line_out)
            if line != line_out:
                print term.color("uuuh %s != %s" %(line, line_out),
                                 'normal', 'red')

        # start check against expected lines
        match = test_against_re.match(line)
        if match:
            check_line_from_process = True

    test_file.close()
