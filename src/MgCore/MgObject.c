
#include "MgObject.h"

#include "MgInterpreter.h"
#include "MgObjectReference.h"


MgStatus* MgObject_represent(MgObject* self, FILE* fs) {
  return self->type->represent(self, fs);
}

MgStatus* MgObject_evaluate(MgObject* self, MgObject** output,
                            MgInterpreter* interpreter,
                            MgEnv* env) {
  MgStatus* s;
  do {
    s = self->type->evaluate(self, output,
                             interpreter,
                             env);
    if ( s != Mg_ok ) return s;

    self = *output;
  } while (Mg_is_a_reference(*output) && s == Mg_ok);
  return s;
}


MgStatus* MgObject_lazy_evaluate(MgObject* self, MgObject** output,
                                 MgInterpreter* interpreter,
                                 MgEnv* env) {
  MgStatus* s;
  s = self->type->evaluate(self, output,
                           interpreter,
                           env);

  return s;
}


MgStatus* MgObject_evaluate_on(MgObject* self,
                               MgObject* target,
                               MgObject** output,
                               MgInterpreter* interpreter,
                               MgEnv* env) {
  MgStatus* s;
  s =  self->type->evaluate_on(self, target, output,
                               interpreter,
                               env);
  if (s != Mg_ok) goto error;

  if (Mg_is_a_reference(*output)) {
    s = MgObject_evaluate(*output, output, interpreter, env);
    if (s != Mg_ok) goto error;
  }

 error:
  return s;
}


MgStatus* MgObject_lazy_evaluate_on(MgObject* self,
                                    MgObject* target,
                                    MgObject** output,
                                    MgInterpreter* interpreter,
                                    MgEnv* env) {
  MgStatus* s;
  s =  self->type->evaluate_on(self, target, output,
                               interpreter,
                               env);
  return s;
}


MgStatus* MgObject_destroy(MgObject* obj) {
  return obj->type->destroy(obj);
}

MgStatus* MgObject_drop_reference(MgObject* obj) {
  if (--obj->refcnt <= 0) {
    return MgObject_destroy(obj);
  }
  return Mg_ok;
}

MgStatus* MgObject_add_reference(MgObject* obj) {
  obj->refcnt++;
  return Mg_ok;
}

static const MgStatus error_object_not_applicable = {
  .message = "object not applicable"
};
const MgStatus* Mg_error_object_not_applicable = &error_object_not_applicable;
