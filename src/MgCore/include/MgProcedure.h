#ifndef MGPROCEDURE_H
#define MGPROCEDURE_H

#include "MgObject.h"
#include "MgList.h"
#include "MgEnvironment.h"

typedef struct MgProcedure MgProcedure;

MgStatus* MgProcedure_create(MgProcedure** procedure,
                             MgList* argument,
                             MgList* code,
                             MgEnv* parent_env);

MgStatus* MgProcedure_destroy(MgProcedure* procedure);

#endif /* MGPROCEDURE_H */
