
#include "MgObject.h"

MgStatus* MgObject_represent(MgObject* self, FILE* fs) {
  return self->type->represent(self, fs);
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
