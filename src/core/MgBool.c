
#include "MgBool.h"
#include "debug.h"


struct MgBool { MgObject base;
  const int value;
};


static MgStatus* not_implemented_too_late(void) {
  return Mg_error_not_implemented;
}

static MgStatus* do_nothing(void) {
  return Mg_ok;
}

static MgStatus* represent(MgBool* bool, FILE* fs) {
  fprintf(fs, "%s", bool->value ? "#t" : "#f");
  return Mg_ok;
}

static const MgObjectType MgBool_type = {
  .evaluate = (MgObject_evaluate_func)not_implemented_too_late,
  .evaluate_on = (MgObject_evaluate_on_func)not_implemented_too_late,
  .represent = (MgObject_represent_func)represent,
  .destroy = (MgObject_destroy_func)do_nothing
};

static MgBool _true = {
  .base = {
    .refcnt = 0,
    .type = &MgBool_type
  },
  .value = 1
};
MgBool* MgBool_true = &_true;

static MgBool _false = {
  .base = {
    .refcnt = 0,
    .type = &MgBool_type
  },
  .value = 0
};
MgBool* MgBool_false = &_false;

int Mg_is_a_bool(MgObject* obj) {
  return obj->type == &MgBool_type ? 1 : 0;
}

int Mg_is_true(MgObject* obj) {
  return obj == (MgObject*)MgBool_true ? 1 : 0;
}

int Mg_is_false(MgObject* obj) {
  return !Mg_is_true(obj);
}

MgStatus* MgBool_parser_func(MgSavedStream* ss,
                             const  MgObjectParser** object_parsers,
                             MgBool** output_bool) {
  int c = MgSavedStream_get_current(ss);

  if (c != '#') {
    return MgParser_error_syntax;
  }

  c = MgSavedStream_get_next(ss);
  if (c < 0) {
      return MgParser_error_getchar;
    }

  if (c == 't') {
    *output_bool = MgBool_true;
  }

  else if (c == 'f') {
    *output_bool = MgBool_false;
  }

  else {
    return MgParser_error_syntax;
  }

  /* exit symbol */
  MgSavedStream_get_next(ss);

  return MgParser_check_symbol_ending(ss);
}

const MgObjectParser MgBool_parser = {
  .parse = (MgObjectParser_func)MgBool_parser_func
};
