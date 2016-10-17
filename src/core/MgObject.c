
#include "MgObject.h"

#include "MgInterpreter.h"

MgStatus* MgObject_represent(MgObject* self, FILE* fs) {
  return self->type->represent(self, fs);
}

MgStatus* MgObject_evaluate(MgObject* self, MgObject** output,
                            MgInterpreter* interpreter,
                            MgEnv* env) {
  return self->type->evaluate(self, output,
                              interpreter,
                              env);
}

MgStatus* MgObject_evaluate_on(MgObject* self,
                               MgObject* target,
                               MgObject** output,
                               MgInterpreter* interpreter,
                               MgEnv* env) {
  return self->type->evaluate_on(self, target, output,
				 interpreter,
				 env);
}

MgStatus* MgObject_destroy(MgObject* obj) {
  /* printf("destroying object:"); */
  /* MgObject_represent(obj, stdout); */
  /* printf("\n"); */
  return obj->type->destroy(obj);
}

MgStatus* MgObject_drop_reference(MgObject* obj) {
  /* printf("dropping object:"); */
  /* if (obj) MgObject_represent(obj, stdout); else printf("NULL"); */
  /* if (obj) printf("from %d\n", obj->refcnt); */
  if (--obj->refcnt <= 0) {
    /* printf("destroying it\n"); */
    return MgObject_destroy(obj);
  }
  return Mg_ok;
}

MgStatus* MgObject_add_reference(MgObject* obj) {
  /* printf("adding object:"); */
  /* if (obj) MgObject_represent(obj, stdout); else printf("NULL"); */
  /* if (obj) printf("from %d\n", obj->refcnt); */
  obj->refcnt++;
  return Mg_ok;
}

static const MgStatus error_object_not_applicable = {
  .message = "object not applicable"
};
const MgStatus* Mg_error_object_not_applicable = &error_object_not_applicable;
