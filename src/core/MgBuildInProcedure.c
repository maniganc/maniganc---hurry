
#include "MgBuildInProcedure.h"

#include <stdlib.h>

struct MgBuildInProcedure { MgObject base;
  MgBuildInProcedure_Func func;
};

static const MgObjectType type = {
  .evaluate = NULL,
  .evaluate_on = NULL,
  .represent = NULL,
  .destroy = NULL,
};

MgStatus* MgBuildInProcedure_create(MgBuildInProcedure_Func func,
				    MgBuildInProcedure** procedure) {
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
