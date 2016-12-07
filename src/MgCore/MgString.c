
#include "MgString.h"
#include "debug.h"
#include <stdlib.h>

/* MgSavedStream has those defs */
/* #define TYPE */
/* #include "vector_template.h" */

struct MgString { MgObject base;
  char* str;
};

static MgStatus * evaluate(MgString * self, MgString ** output){
  *output = self;
  return Mg_ok;
}

static MgStatus* evaluate_on(MgString * self, MgString** output){
  return Mg_error_object_not_applicable;
}

static MgStatus* represent(MgString* string, FILE* fs) {
  fprintf(fs, "\"%s\"", string->str);
  return Mg_ok;
}

static MgStatus* destroy(MgString* string) {
  free(string->str);
  free(string);
  return Mg_ok;
}

static const MgObjectType MgString_type = {
  .evaluate = (MgObject_evaluate_func)evaluate,
  .evaluate_on = (MgObject_evaluate_on_func)evaluate_on,
  .represent = (MgObject_represent_func)represent,
  .destroy = (MgObject_destroy_func)destroy
};

MgStatus* MgString_create_from_string(MgString** string, char* str) {
  MgString* new_string = malloc(sizeof(MgString));
  if (new_string == NULL) {
    return Mg_error_malloc;
  }

  new_string->base.refcnt = 0;
  new_string->base.type = &MgString_type;
  new_string->str = strdup(str);

  *string = new_string;

  return Mg_ok;
}

const char* MgString_get_string(const MgString* string) {
  return string->str;
}

int Mg_is_a_string(const MgObject* obj) {
  return obj->type == &MgString_type ? 1 : 0;
}

MgStatus* MgString_parser_func(MgSavedStream* ss,
                               const  MgObjectParser** object_parsers,
                               MgString** output_string) {
  /* do not create an objet yet : due to bad symbol ending,
   * invalid objects would have to be freed if created */
  static int string_cnt = 0;
  
  vector_char temp_str;
  vector_char_init(&temp_str);

  int c = MgSavedStream_get_current(ss);
  if (c != '\"') {
    vector_char_deinit(&temp_str);
    return MgParser_error_syntax;
  }
  

  MgSavedStream_get_next(ss);
  if (c < 0) {
    vector_char_deinit(&temp_str);
    return MgParser_error_getchar;
  }

  do {
    c = MgSavedStream_get_current(ss);

    if (c == '\"') {
      /* end of string */
      c = MgSavedStream_get_next(ss);
      if (c < 0) {
        vector_char_deinit(&temp_str);
        return MgParser_error_getchar;
      }
      break;
    }

    else if (c == '\\') {
      /* next char is special  */
      c = MgSavedStream_get_next(ss);
      if (c < 0) {
        vector_char_deinit(&temp_str);
        return MgParser_error_getchar;
      }
      vector_char_push(&temp_str, c);
    }

    else {
      /* normal char */
      vector_char_push(&temp_str, c);
    }

    /* next char */
    c = MgSavedStream_get_next(ss);
    if (c < 0) {
      vector_char_deinit(&temp_str);
      return MgParser_error_getchar;
    }
  }
  while(1);

  /* finish it to from a valid string */
  vector_char_push(&temp_str, '\0');

  /* already outside symbol */
  /* check ending */
  MgStatus* ending_status =  MgParser_check_symbol_ending(ss);

  if (ending_status == Mg_ok) {
    /* return creation status */
    MgStatus* s;
    s = MgString_create_from_string(output_string,
                                    vector_char_get_raw_ptr(&temp_str));
    vector_char_deinit(&temp_str);
    return s;
  }
  else {
    vector_char_deinit(&temp_str);
    return MgParser_error_syntax;
  }
}

const MgObjectParser MgString_parser = {
  .parse = (MgObjectParser_func)MgString_parser_func
};
