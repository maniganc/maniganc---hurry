
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

/* char* test = "(#t 10029 :e- ()e-e() #\\A #\\newline #\\space fooo #\\a ) "; */
char* test = "(#\\newliner #\\space)";

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

int main(int argc, char *argv[]) {
#ifdef DEBUG
  puts("-- Build date: "STR(BUILD_INFO_DATE));
  puts("-- Build commit: "STR(BUILD_INFO_COMMIT));
#endif

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
      printf("error: %s\n", s->message);
      stop = 1;
    }

    else {
      printf("==> ");
      MgObject_represent(output_object, stdout);
    }


    printf("\n$ ");
  }
  return 0;
}
