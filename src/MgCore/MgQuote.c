
#include "MgQuote.h"
#include "MgIdentifier.h"

#include "debug.h"
#include <stdlib.h>

MgStatus* MgQuote_parser_func(MgSavedStream* ss,
                              const  MgObjectParser** object_parsers,
                              MgList** output_list) {

  int c = MgSavedStream_get_current(ss);
  if (c != '\'') {
    return MgParser_error_syntax;
  }

  c = MgSavedStream_get_next(ss);
  if (c < 0) {
    return MgParser_error_getchar;
  }

  /* parse quoted object */
  MgObject* obj;
  MgStatus* s = MgParser_parse_object(ss, object_parsers, &obj);
  if (s != Mg_ok) {
    return MgParser_error_syntax;
  }

  /* create identifier */
  MgIdentifier* quote;
  s = MgIdentifier_create_from_string(&quote, "quote");
  if (s != Mg_ok) {
    return s;
  }

  /* create list */
  MgList* list;
  s = MgList_create(&list);
  if (s != Mg_ok) {
    return s;
  }

  /* create (quote <obj>) */
  s = MgList_push_front(&list, obj);
  if (s != Mg_ok) {
    return s;
  }
  s = MgList_push_front(&list, (MgObject*)quote);
  if (s != Mg_ok) {
    return s;
  }

  *output_list = list;

  return Mg_ok;
}

const MgObjectParser MgQuote_parser = {
  .parse = (MgObjectParser_func)MgQuote_parser_func
};

int Mg_is_quote(MgObject* obj) {
  if (Mg_is_an_identifier(obj)) {
    MgIdentifier* id = (MgIdentifier*)obj;
    if (strcmp(MgIdentifier_get_name(id), "quote") == 0) {
      return 1;
    }
  }
  return 0;
}
