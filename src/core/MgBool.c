
#include "MgBool.h"
#include "debug.h"

struct MgBool { MgObject base;
  const int value;
};

static MgStatus* evaluate(MgBool* self, MgBool** output) {
  *output = self;
  return Mg_ok;
}

static MgStatus* evaluate_on(MgBool* self, MgBool** output) {
  return Mg_error_object_not_applicable;
}

static MgStatus* represent(MgBool* self, FILE* fs) {
  fprintf(fs, "%s", self->value ? "#t" : "#f");
  return Mg_ok;
}

static MgStatus* destroy(MgBool* self) {
  return Mg_ok;
}

static const MgObjectType MgBool_type = {
  .evaluate = (MgObject_evaluate_func)evaluate,
  .evaluate_on = (MgObject_evaluate_on_func)evaluate_on,
  .represent = (MgObject_represent_func)represent,
  .destroy = (MgObject_destroy_func)destroy
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
  return !Mg_is_false(obj);
}

int Mg_is_false(MgObject* obj) {
  return obj == (MgObject*)MgBool_false ? 1 : 0;
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
