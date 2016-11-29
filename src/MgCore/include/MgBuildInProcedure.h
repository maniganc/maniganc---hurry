#ifndef MGBUILDINPROCEDURE_H
#define MGBUILDINPROCEDURE_H

#include "MgObject.h"

typedef struct MgBuildInProcedure MgBuildInProcedure;

typedef MgStatus* (*MgBuildInProcedure_Func)(MgObject* arg,
					     MgObject** output,
					     MgInterpreter* interpreter,
					     MgEnv* env);

MgStatus* MgBuildInProcedure_create(MgBuildInProcedure** procedure,
				    MgBuildInProcedure_Func func);

MgStatus* MgBuildInProcedure_destroy(MgBuildInProcedure* procedure);

#endif /* MGBUILDINPROCEDURE_H */
