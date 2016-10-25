
#include "MgProcedure.h"

#include <stdlib.h>

struct MgProcedure { MgObject base;
  MgProcedure_Global_Func func;
};

static MgStatus * evaluate(MgProcedure* procd, MgObject** output,
                           MgInterpreter* interpreter,
                           MgEnv* Env) {
  *output = (MgObject*)procd;
  return Mg_ok;
}

static MgStatus* evaluate_on(MgProcedure* procd,
                       MgProcedure* target,
                       MgObject** output,
                       MgInterpreter* interpreter,
                       MgEnv* env) {
  return procd->func(target, output, interpreter, env); 
}

MgStatus* represent(MgProcedure* procd, FILE* fs) {
  fprintf(fs, "<build-in procedure @ %p>", procd);
  return Mg_ok;
};

MgStatus* destroy(MgProcedure* procd){
  return MgProcedure_destroy(procd);
}

static const MgObjectType type = {
  .evaluate = (MgObject_evaluate_func)evaluate,
  .evaluate_on = (MgObject_evaluate_on_func)evaluate_on,
  .represent = (MgObject_represent_func)represent,
  .destroy = (MgObject_destroy_func)destroy
};

MgStatus * MgProcedure_create(MgProcedure** procedure, 
                              MgProcedure_Global_Func func) {
  MgProcedure* new_procedure = malloc(sizeof(MgProcedure));

  if (new_procedure == NULL) {
  	return Mg_error_malloc;
  }

  new_procedure->base.refcnt = 0;
  new_procedure->base.type = &type;

  new_procedure->func = func;

  *procedure = new_procedure;
  return Mg_ok;
}

MgStatus* MgProcedure_destroy(MgProcedure* procd) {
  free(procd);
  return Mg_ok;
}