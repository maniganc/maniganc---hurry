
#include "MgObject.h"

MgStatus* MgObject_represent(MgObject* self, FILE* fs) {
  return self->type->represent(self, fs);
}

MgStatus* MgObject_evaluate(MgObject* self, MgObject** output) {
  return self->type->evaluate(self, output);
}

MgStatus* MgObject_evaluate_on(MgObject* self,
			       MgObject* target,
			       MgObject** output) {
  return self->type->evaluate_on(self, target, output);
}

MgStatus* MgObject_destroy(MgObject* obj) {
  return obj->type->destroy(obj);
}

MgStatus* MgObject_drop(MgObject* obj) {
  if (--obj->refcnt <= 0) {
    return MgObject_destroy(obj);
  }
  return Mg_ok;
}

static const MgStatus error_object_not_applicable = {
  .message = "object not applicable"
};
const MgStatus* Mg_error_object_not_applicable = &error_object_not_applicable;

