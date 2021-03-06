
#include "MgChar.h"
#include "debug.h"
#include <stdlib.h>

struct MgChar { MgObject base;
  char value;
};

static MgStatus* evaluate(MgChar* self, MgChar** output) {
  *output = self;
  return Mg_ok;
}

static MgStatus* evaluate_on(MgChar* self, MgChar** output) {
  return Mg_error_object_not_applicable;
}

static MgStatus* represent(MgChar* ch, FILE* fs) {
  if (ch->value == ' ') {
    fprintf(fs, "#\\space");
  }
  else if (ch->value == '\n') {
    fprintf(fs, "#\\newline");
  }
  else {
    fprintf(fs, "#\\%c", ch->value);
  }
  return Mg_ok;
}

static MgStatus* destroy(MgChar* ch) {
  free(ch);
  return Mg_ok;
}

static const MgObjectType MgChar_type = {
    .evaluate = (MgObject_evaluate_func)evaluate,
  .evaluate_on = (MgObject_evaluate_on_func)evaluate_on,
  .represent = (MgObject_represent_func)represent,
  .destroy = (MgObject_destroy_func)destroy
};

MgStatus* MgChar_create(MgChar** ch, char c) {
  MgChar* new_char = malloc(sizeof(MgChar));
  if (new_char == NULL) {
    return Mg_error_malloc;
  }

  new_char->base.refcnt = 0;
  new_char->base.type = &MgChar_type;
  new_char->value = c;

  *ch = new_char;

  return Mg_ok;
}

int Mg_is_a_char(const MgObject* obj) {
  return obj->type == &MgChar_type ? 1 : 0;
}

char MgChar_get_char(const MgChar* obj) {
  return obj->value;
}

static MgStatus* MgParse_parse_alpha_string(MgSavedStream*ss, char** s) {
  vector_char tmp_str;
  vector_char_init(&tmp_str);

  int c;
  do {
    c = MgSavedStream_get_current(ss);
    if ( ('a' <= c && c <= 'z') ||
         ('A' <= c && c <= 'Z')) {
      vector_char_push(&tmp_str, c);
    }
    else {
      break;
    }

    c = MgSavedStream_get_next(ss);
    if (c < 0) {
      return MgParser_error_getchar;
    }
  }
  while (1);

  /* zero termination */
  vector_char_push(&tmp_str, '\0');

  char* str = strdup(vector_char_get_raw_ptr(&tmp_str));

  vector_char_deinit(&tmp_str);

  *s = str;

  return Mg_ok;
}

MgStatus* MgChar_parser_func(MgSavedStream* ss,
                             const  MgObjectParser** object_parsers,
                             MgChar** output_char) {
  /* do not create an objet yet : due to bad symbol ending,
   * object would have to be freed if created */
  char value;

  int c = MgSavedStream_get_current(ss);

  if (c != '#') {
    return MgParser_error_syntax;
  }

  /* next char */
  c = MgSavedStream_get_next(ss);
  if (c < 0) {
    return MgParser_error_getchar;
  }
  else if (c != '\\') {
    return MgParser_error_syntax;
  }

  c = MgSavedStream_get_next(ss);
  if (c < 0) {
    return MgParser_error_getchar;
  }
  else if ( ('a' >= c || c >= 'z') &&
	    ('A' >= c || c >= 'Z')) {
    value = c;
    c = MgSavedStream_get_next(ss);
    if (c < 0) {
      return MgParser_error_getchar;
    }
  }
  else {
    char* name;
    MgStatus* s = MgParse_parse_alpha_string(ss, &name);
    if (s != Mg_ok) {
      return s;
    }

    if (strlen(name) == 0) {
      value = ' ';
    }
    else if (strlen(name) == 1) {
      value = name[0];
    }
    else if (strcasecmp(name, "space") == 0) {
      value = ' ';
    }
    else if (strcasecmp(name, "newline") == 0) {
      value = '\n';
    }
    else {
      free(name);
      return MgParser_error_syntax;
    }
    free(name);
  }

  /* already outside symbol */
  MgStatus* ending_status =  MgParser_check_symbol_ending(ss);

  if (ending_status == Mg_ok) {
    /* return creation status */
    return MgChar_create(output_char, value);
  }
  else {
    return MgParser_error_syntax;
  }
}

const MgObjectParser MgChar_parser = {
  .parse = (MgObjectParser_func)MgChar_parser_func
};
