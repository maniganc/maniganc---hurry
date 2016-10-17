
#include <stdio.h>

#include "MgList.h"
#include "MgBool.h"
#include "MgInteger.h"
#include "MgIdentifier.h"
#include "MgChar.h"
#include "MgString.h"
#include "MgQuote.h"

#include "debug.h"

#define SSTR(x) #x
#define STR(x) SSTR(x)

/* int func_get_next_char(void* payload) { */
/*   return getchar(); */
/* } */

/* static const char* in = "\"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\""; */

/* int func_get_next_char(void* payload) { */
/*   /\* static size_t idx = 0; *\/ */
/*   /\* if (idx == 1302) { *\/ */
/*   /\*   debug("end\n"); *\/ */
/*   /\*   return ' '; *\/ */
/*   /\* } *\/ */
/*   /\* debug("send char in  stream %c\n", in[idx]); *\/ */
/*   /\* return in[idx++]; *\/ */
/*   return getchar(); */
/* } */

/* extern const MgObjectParser* object_parsers[]; */

/* static void interactive(void) { */
/* /\* #ifdef DEBUG *\/ */
/* /\*   puts("-- Build date: "STR(BUILD_INFO_DATE)); *\/ */
/* /\*   puts("-- Build commit: "STR(BUILD_INFO_COMMIT)); *\/ */
/* /\* #endif // DEBUG *\/ */
/*   MgObject* output_object; */
/*   MgSavedStream ss; */
/*   printf("$ "); */
/*   MgSavedStream_init(&ss, func_get_next_char, NULL); */

/*   while(1) { */
/*     if (MgSavedStream_get_current(&ss) == EOF) { */
/*       break; */
/*     } */

/*     MgParser_skip_sugar(&ss, NULL); */

/*     if (MgSavedStream_get_current(&ss) == EOF) { */
/*       break; */
/*     } */

/*     MgStatus* s =  MgParser_parse_object(&ss, object_parsers, &output_object); */
/*     if (s != Mg_ok) { */
/*       fprintf(stderr, "error at line %ld: %s\n", */
/*            MgSavedStream_get_line_number(&ss), */
/*            s->message); */

/*       /\* flush stdin *\/ */
/*       int ch; while ((ch = getchar()) != '\n' && ch != EOF); */
/*       MgSavedStream_reset(&ss); */
/*     } */

/*     else { */
/*       printf("==> "); */
/* #ifdef DEBUG */
/*       MgObject_represent(output_object, stdout); */
/* #endif // DEBUG */
/*       MgObject* evaluated_obj; */
/*       s = MgObject_evaluate(output_object, &evaluated_obj); */
/*       if (s != Mg_ok) { */
/*         /\* failed to represent output object *\/ */
/*         fprintf(stderr, "error: %s\n", s->message); */

/*       } */
/*       else { */
/*         MgObject_represent(output_object, stdout); */
/*         /\* for now, evaluated object is useless, delete it *\/ */
/*      if (evaluated_obj != output_object) { */
/*        /\* evaluated object can be the same as output object */
/*         * avoid double-free *\/ */
/*        MgObject_destroy(evaluated_obj); */
/*      } */
/*       } */
/*       MgObject_destroy(output_object); /\* for now, object is useless *\/ */
/*     } */


/*     printf("\n$ "); */
/*   } */

/*   MgSavedStream_deinit(&ss); */
/* } */

/* static int parse_filestream(FILE* fs) { */
/*   MgSavedStream ss; */
/*   MgSavedStream_init(&ss, (MgSavedStream_getchar_func)fgetc, fs); */

/*   MgObject* output_object; */

/*   int stop = 0; */
/*   while(!stop) { */

/*     if (MgSavedStream_get_current(&ss) == EOF) { */
/*       break; */
/*     } */

/*     MgParser_skip_sugar(&ss, NULL); */

/*     if (MgSavedStream_get_current(&ss) == EOF) { */
/*       break; */
/*     } */

/*     MgStatus* s =  MgParser_parse_object(&ss, object_parsers, &output_object); */
/*     if (s != Mg_ok) { */
/*       fprintf(stderr, "error: %s\n", s->message); */
/*       MgSavedStream_deinit(&ss); */
/*       return -1; */
/*     } */

/*     else { */
/*       MgObject_represent(output_object, stdout); */
/*       printf("\n");                    /\* separate representations *\/ */
/*       MgObject_destroy(output_object); /\* for now, object is useless */
/*                                         * no need to keep it in memory *\/ */
/*     } */
/*   } */

/*   MgSavedStream_deinit(&ss); */
/*   return 0; */
/* } */

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
  return -1;
}
