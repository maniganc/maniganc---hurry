
#include "MgObjectReference.h"

#include "MgInterpreter.h"

#include <stdlib.h>

struct MgObjectReference { MgObject base;
  MgObject* obj;
  MgEnv* env;
};

static MgStatus* evaluate(MgObjectReference* self, MgObject** output,
			  MgInterpreter* interpreter,
			  MgEnv* env){
  return MgObject_lazy_evaluate(self->obj, output, interpreter, self->env);
}

static MgStatus* error(void) {
  return Mg_error;
}

static const MgObjectType MgObjectReference_type = {
  .evaluate = (MgObject_evaluate_func)evaluate,
  .evaluate_on = (MgObject_evaluate_on_func)error,
  .represent = (MgObject_represent_func)error,
  .destroy = (MgObject_destroy_func)error
};


MgStatus* MgObjectReference_create(MgObjectReference** ref) {
  MgObjectReference* new_ref = malloc(sizeof(MgObjectReference));
  if (new_ref == NULL) {
    return Mg_error_malloc;
  }
  new_ref->base.refcnt = 0;
  new_ref->base.type = &MgObjectReference_type;
  new_ref->env = NULL;
  new_ref->obj = NULL;

  *ref = new_ref;
  
  return Mg_ok;
}

MgStatus* MgObjectReference_destroy(MgObjectReference* ref) {
  /* drop previous env & object */
  if (ref->env) MgObject_drop_reference((MgObject*)ref->env);
  if (ref->obj) MgObject_drop_reference((MgObject*)ref->obj);
  
  free(ref);
  return Mg_ok;
}

int Mg_is_a_reference(const MgObject* obj) {
  if (obj->type == &MgObjectReference_type) {
    return 1;
  }
  return 0;
}

MgStatus* MgObjectReference_return_ref(MgObject* object,
                                       MgObjectReference** object_ref,
                                       MgInterpreter* interpreter,
                                       MgEnv* env) {

  MgObjectReference* ref = MgInterpreter_get_reference(interpreter);

  /* drop previous env & object */
  if (ref->env) MgObject_drop_reference((MgObject*)ref->env);
  if (ref->obj) MgObject_drop_reference((MgObject*)ref->obj);

  ref->obj = object;
  ref->env = env;

  /* own new env & object */
  MgObject_add_reference((MgObject*)object);
  MgObject_add_reference((MgObject*)env);

  *object_ref = ref;
  
  return Mg_ok;
}
