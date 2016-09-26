
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
  printf("hello\n");
  puts("Build date: "STR(BUILD_INFO_DATE));
  puts("Build commit: "STR(BUILD_INFO_COMMIT));

  MgObject* output_object;
  MgSavedStream ss;
  MgSavedStream_init(&ss, func_get_next_char, NULL);
  MgStatus* s =  MgParser_parse_object(&ss, object_parsers, &output_object);
  if (s != Mg_ok) {
    printf("error: %s\n", s->message);
    return -1;
  }
  else {
    printf("valid!\n");
    printf(">");
    MgObject_represent(output_object, stdout);
    printf("<");
  }
  return 0;
}
