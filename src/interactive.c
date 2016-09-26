
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
}

int main(int argc, char *argv[]) {
#ifdef DEBUG
  puts("-- Build date: "STR(BUILD_INFO_DATE));
  puts("-- Build commit: "STR(BUILD_INFO_COMMIT));
#endif

  if (argc == 1) {
    interactive();
    return 0;
  }

  
  
  return 0;
}
