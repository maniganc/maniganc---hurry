
#include "MgInterpreter.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

int main(int argc, char *argv[]) {

  struct option long_options[] = {
    {"parse-only",   no_argument,       0, 'p' },
    {"execute-file", required_argument, 0, 'f' },
    {"interactive",  no_argument,       0, 'i' },
    {"help",         no_argument,       0, 'h' },
    {0, 0, 0, 0 }
  };

  int parse_only_flag = 0;

  int execute_file_flag = 0;
  char* filename = NULL;

  int interactive_flag = 0;

  int opt;
  int long_index;
  while ((opt = getopt_long(argc, argv,"pf:ih", long_options, &long_index )) != -1) {
    switch (opt) {
    case 'p':
      parse_only_flag = 1;
      break;

    case 'f':
      execute_file_flag = 1;
      filename = strdup(optarg);
      break;

    case 'i':
      interactive_flag = 1;
      break;

    case 'h':
    default:
      printf("%s\n -p(--parse-only)\n -f(--execute_file)\n -i(--interactive)\n -h(--help)\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  MgStatus* status;

  MgInterpreter* interpreter;
  status = MgInterpreter_create(&interpreter);
  if (status != Mg_ok) {
    fprintf(stderr, "failed to create interpreter, error %s\n",
            status->message);
    goto free_filename_and_error;
  }

  /* interactive */
  if (!execute_file_flag) {
    status = MgInterpreter_evaluate_stream(interpreter, stdin,
                                           1,
                                           parse_only_flag);
    if (status != Mg_ok) {
      fprintf(stderr, "failed to evaluate stdin, error %s\n",
              status->message);
      goto free_interpreter_and_others_and_error;
    }
    goto exit_ok;
  }

  /* fileexec */
  if (execute_file_flag) {
    FILE* fs = fopen(filename, "rt");
    if (fs == NULL) {
      fprintf(stderr, "failed to open file %s\n",
              filename);
      goto free_interpreter_and_others_and_error;
    }
    status = MgInterpreter_evaluate_stream(interpreter, fs,
                                           interactive_flag,
                                           parse_only_flag);
    if (status != Mg_ok) {
      fprintf(stderr, "failed to evaluate file, error %s\n",
              status->message);
      goto free_interpreter_and_others_and_error;
    }
    goto exit_ok;
  }

  exit_ok:
  if (filename) free(filename);
  MgInterpreter_destroy(interpreter);
  return 0;

  free_interpreter_and_others_and_error:
  MgInterpreter_destroy(interpreter);
  free_filename_and_error:
  if (filename) free(filename);
  error:
  return 1; 	   
}
