
#include "MgBuildInProcedure.h"

#include <stdlib.h>

struct MgBuildInProcedure { MgObject base;
  MgBuildInProcedure_Func func;
};

static MgStatus* evaluate(MgBuildInProcedure* proc, MgObject** output,
			  MgInterpreter* interpreter,
			  MgEnv* env) {
  *output = (MgObject*)proc;
  return Mg_ok;
}

static MgStatus* evaluate_on(MgBuildInProcedure* proc,
                             MgObject* target,
                             MgObject** output,
			     MgInterpreter* interpreter,
			     MgEnv* env) {
  return proc->func(target, output, interpreter, env);
}

static MgStatus* represent(MgBuildInProcedure* proc,
                           FILE* fs) {
  fprintf(fs, "<build-in procedure @ %p>", (void*)proc);
  return Mg_ok;
}

static MgStatus* destroy(MgBuildInProcedure* proc) {
  return MgBuildInProcedure_destroy(proc);
}

static const MgObjectType type = {
  .evaluate = (MgObject_evaluate_func)evaluate,
  .evaluate_on = (MgObject_evaluate_on_func)evaluate_on,
  .represent = (MgObject_represent_func)represent,
  .destroy = (MgObject_destroy_func)destroy
};

MgStatus* MgBuildInProcedure_create(MgBuildInProcedure** procedure,
				    MgBuildInProcedure_Func func) {
  MgBuildInProcedure* new_procedure = malloc(sizeof(MgBuildInProcedure));
  if (new_procedure == NULL) {
    return Mg_error_malloc;
  }

  /* base fields */
  new_procedure->base.refcnt = 0;
  new_procedure->base.type = &type;
  /* object fields*/
  new_procedure->func = func;

  *procedure = new_procedure;
  return Mg_ok;
}

MgStatus* MgBuildInProcedure_destroy(MgBuildInProcedure* procedure) {
  free(procedure);
  return Mg_ok;
}
