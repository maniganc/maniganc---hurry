
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
/* 	      MgSavedStream_get_line_number(&ss), */
/* 	      s->message); */

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
/* 	if (evaluated_obj != output_object) { */
/* 	  /\* evaluated object can be the same as output object */
/* 	   * avoid double-free *\/ */
/* 	  MgObject_destroy(evaluated_obj); */
/* 	} */
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

int main(int argc, char *argv[]) {
  MgStatus* status;
  
  MgInterpreter* interpreter;
  status = MgInterpreter_create(&interpreter);
  if (status != Mg_ok) {
    fprintf(stderr, "failed to create interpreter, error %s\n",
	    status->message);
    return -1;
  }

  status = MgInterpreter_evaluate_stream(interpreter, stdin, 1);
  if (status != Mg_ok) {
    fprintf(stderr, "failed to evaluate stream, error %s\n",
	    status->message);
    return -1;
  }

  MgInterpreter_destroy(interpreter);
  return 0;
  
  if (argc == 1) {
    interactive();
    return 0;
  }

  /* there is an argument, probably a file */
  FILE* fs = fopen(argv[1], "rt");
  return parse_filestream(fs);
}
