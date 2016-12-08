
#include "MgUnspecified.h"
#include "debug.h"

struct MgUnspecified { MgObject base;
};

static MgStatus* evaluate(MgUnspecified* self, MgUnspecified** output) {
  *output = self;
  return Mg_ok;
}

static MgStatus* evaluate_on(MgUnspecified* self, MgUnspecified** output) {
  return Mg_error_object_not_applicable;
}

static MgStatus* represent(MgUnspecified* self, FILE* fs) {
  fprintf(fs, "<unspecified>");
  return Mg_ok;
}

static MgStatus* destroy(MgUnspecified* self) {
  return Mg_ok;
}

static const MgObjectType MgUnspecified_type = {
  .evaluate = (MgObject_evaluate_func)evaluate,
  .evaluate_on = (MgObject_evaluate_on_func)evaluate_on,
  .represent = (MgObject_represent_func)represent,
  .destroy = (MgObject_destroy_func)destroy
};

static MgUnspecified unspecified = {
  .base = {
    .refcnt = 0,
    .type = &MgUnspecified_type
  }
};
MgUnspecified* Mg_unspecified = &unspecified;
