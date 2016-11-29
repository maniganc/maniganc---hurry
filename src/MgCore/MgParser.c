
#include "MgParser.h"

static const MgStatus error_syntax = {
  .message = "syntax error"
};
const MgStatus* MgParser_error_syntax = &error_syntax;


static const MgStatus error_getchar = {
  .message = "error while getting the next char"
};
const MgStatus* MgParser_error_getchar = &error_getchar;

MgStatus* MgParser_skip_line(MgSavedStream* ss) {
  int c;
  do {
    c = MgSavedStream_get_current(ss);
    if (c < 0) {
      return MgParser_error_getchar;
    }

    if (c != '\n') {
      MgSavedStream_get_next(ss);
    }
  }
  while(c != '\n');

  return Mg_ok;
}

MgStatus* MgParser_skip_sugar(MgSavedStream* ss, int* whitespace_cnt) {
  int stop = 0;
  int whitespace_cnt_int = 0;
  int c;
  do {
    c = MgSavedStream_get_current(ss);
    if (c < 0) {
      return MgParser_error_getchar;
    }
    switch (c) {
    case ' ':
    case '\t':
    case '\n':
      /* whitespace */
      whitespace_cnt_int++;
      break;

    case ';': {
      /* a comment, skip char until newline */
      MgStatus* s = MgParser_skip_line(ss);
      if (s != Mg_ok) {
        return s;
      }
      whitespace_cnt_int++;
      break;
    }

    default: {
      /* not sugar */
      stop = 1;
    }
      break;
    }

    if (!stop) {
      /* next char */
      MgSavedStream_get_next(ss);
    }
  }
  while(!stop);

  if (whitespace_cnt != NULL) {
    *whitespace_cnt = whitespace_cnt_int;
  }

  return Mg_ok;
}

MgStatus* MgParser_parse_object(MgSavedStream* ss,
                                const MgObjectParser** object_parsers,
                                MgObject** output_object) {
  /* mark the current char so we can go back to it if the parser fails */
  MgSavedStream_mark m = MgSavedStream_mark_current(ss);

  /* iterate on parsers */
  const MgObjectParser** parser = object_parsers;
  while (*parser != NULL) {
    /* try parser */
    MgObject* obj = NULL;
    MgStatus* s = (*parser)->parse(ss, object_parsers, &obj);
    if (s == Mg_ok) {
      /* object successfully parsed */
      MgSavedStream_drop_mark(ss);
      *output_object = obj;
      return Mg_ok;
    }

    else if (s == MgParser_error_syntax) {
      /* parser cannot parse it  */
      /* go back to the first char */
      MgSavedStream_load_mark(ss, m);
    }

    else {
      /* err */
      /* drop mark, and get out */
      MgSavedStream_drop_mark(ss);
      return s;
    }

    /* next parser */
    parser++;
  }

  /* failed to find a suitable parser */
  MgSavedStream_drop_mark(ss);
  return MgParser_error_syntax;
}

MgStatus* MgParser_check_symbol_ending(MgSavedStream* ss) {
  /* check if there is some whitespaces/paren after it,
   * thus not any other sympbols that tailgate it*/

  int c = MgSavedStream_get_current(ss);

  switch (c) {
  case ')':
  case '(':
  case ' ':
  case EOF:
  case '\0':
  case '\n':
  case ';':
    return Mg_ok;
    
  default:
    return MgParser_error_syntax;
  }
}
