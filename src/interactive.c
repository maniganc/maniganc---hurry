
#include <stdio.h>

#include "MgList.h"
#include "MgBool.h"
#include "MgInteger.h"
#include "MgIdentifier.h"
#include "MgChar.h"
#include "MgString.h"
#include "MgQuote.h"

#define SSTR(x) #x
#define STR(x) SSTR(x)

int func_get_next_char(void* payload) {
  return getchar();
}

const MgObjectParser* object_parsers[] = {
  &MgList_parser,
  &MgBool_parser,
  &MgInteger_parser,
  &MgIdentifier_parser,
  &MgChar_parser,
  &MgString_parser,
  &MgQuote_parser,
  NULL
};

static void interactive(void) {
  puts("-- Build date: "STR(BUILD_INFO_DATE));
  puts("-- Build commit: "STR(BUILD_INFO_COMMIT));
  MgObject* output_object;
  MgSavedStream ss;
  printf("$ ");
  MgSavedStream_init(&ss, func_get_next_char, NULL);

  int stop = 0;
  while(!stop) {

    if (MgSavedStream_get_current(&ss) == EOF) {
      break;
    }

    MgParser_skip_sugar(&ss, NULL);

    if (MgSavedStream_get_current(&ss) == EOF) {
      break;
    }

    MgStatus* s =  MgParser_parse_object(&ss, object_parsers, &output_object);
    if (s != Mg_ok) {
      fprintf(stderr, "error: %s\n", s->message);

      /* flush stdin */
      int ch; while ((ch = getchar()) != '\n' && ch != EOF);
      MgSavedStream_reset(&ss);
    }

    else {
      printf("==> ");
      MgObject_represent(output_object, stdout);
      MgObject_destroy(output_object); /* for now, object is useless */
    }


    printf("\n$ ");
  }

  MgSavedStream_deinit(&ss);
}

static int parse_filestream(FILE* fs) {
  MgSavedStream ss;
  MgSavedStream_init(&ss, (MgSavedStream_getchar_func)fgetc, fs);

  MgObject* output_object;

  int stop = 0;
  while(!stop) {

    if (MgSavedStream_get_current(&ss) == EOF) {
      break;
    }

    MgParser_skip_sugar(&ss, NULL);

    if (MgSavedStream_get_current(&ss) == EOF) {
      break;
    }

    MgStatus* s =  MgParser_parse_object(&ss, object_parsers, &output_object);
    if (s != Mg_ok) {
      fprintf(stderr, "error: %s\n", s->message);
      MgSavedStream_deinit(&ss);
      return -1;
    }

    else {
      MgObject_represent(output_object, stdout);
      printf("\n");                    /* separate representations */
      MgObject_destroy(output_object); /* for now, object is useless
                                        * no need to keep it in memory */
    }
  }

  MgSavedStream_deinit(&ss);
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    interactive();
    return 0;
  }

  /* there is an argument, probably a file */
  FILE* fs = fopen(argv[1], "rt");
  return parse_filestream(fs);
}
