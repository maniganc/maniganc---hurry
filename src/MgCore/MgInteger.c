
#include "MgInteger.h"
#include "debug.h"
#include <stdlib.h>

struct MgInteger { MgObject base;
  int value;
};

static MgStatus* evaluate(MgInteger* self, MgInteger** output) {
  *output = self;
  return Mg_ok;
}

static MgStatus* evaluate_on(MgInteger* self, MgInteger** output) {
  return Mg_error_object_not_applicable;
}

static MgStatus* represent(MgInteger* integer, FILE* fs) {
  fprintf(fs, "%d", integer->value);
  return Mg_ok;
}

static MgStatus* destroy(MgInteger* integer) {
  free(integer);
  return Mg_ok;
}

static const MgObjectType MgInteger_type = {
  .evaluate = (MgObject_evaluate_func)evaluate,
  .evaluate_on = (MgObject_evaluate_on_func)evaluate_on,
  .represent = (MgObject_represent_func)represent,
  .destroy = (MgObject_destroy_func)destroy
};

MgStatus* MgInteger_create_from_int(MgInteger** integer, int value) {
  MgInteger* new_integer = malloc(sizeof(MgInteger));
  if (new_integer == NULL) {
    return Mg_error_malloc;
  }

  new_integer->base.refcnt = 0;
  new_integer->base.type = &MgInteger_type;
  new_integer->value = value;

  *integer = new_integer;

  return Mg_ok;
}

int Mg_is_an_integer(const MgObject* obj) {
  return obj->type == &MgInteger_type ? 1 : 0;
}

int MgInteger_get_value(const MgInteger* obj) {
  return obj->value;
}

MgStatus* MgInteger_parser_func(MgSavedStream* ss,
                                const  MgObjectParser** object_parsers,
                                MgInteger** output_integer) {
  /* do not create an objet yet : due to bad symbol ending,
   * object would have to be freed if created */
  int value = 0;
  int invert_coeff = 1;         /* positive */

  int numdigits = 0; 		/* avoid to match empty numbers */

  int c = MgSavedStream_get_current(ss);

  if (c == '+') {
    /* positive is by default*/
    MgSavedStream_get_next(ss);
    if (c < 0) {
      return MgParser_error_getchar;
    }
  }
  else if (c == '-') {
    invert_coeff = -1;
    MgSavedStream_get_next(ss);
    if (c < 0) {
      return MgParser_error_getchar;
    }
  }

  do {
    c = MgSavedStream_get_current(ss);
    if ( '0' <= c && c <= '9') {
      /* a digit */
      value *= 10;
      value += c - '0';
      numdigits++;
      MgSavedStream_get_next(ss);
    }

    else {
      /* something else */
      break;
    }

    
    if (c < 0) {
      return MgParser_error_getchar;
    }
  }
  while(1);

  if (numdigits == 0) {
    return MgParser_error_syntax;
  }

  value *= invert_coeff;

  /* already outside symbol */
  MgStatus* ending_status =  MgParser_check_symbol_ending(ss);

  if (ending_status == Mg_ok) {
    /* return creation status */
    return MgInteger_create_from_int(output_integer, value);
  }
  else {
    return MgParser_error_syntax;
  }
}

const MgObjectParser MgInteger_parser = {
  .parse = (MgObjectParser_func)MgInteger_parser_func
};
