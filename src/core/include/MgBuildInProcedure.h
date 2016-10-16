#ifndef MGBUILDINPROCEDURE_H
#define MGBUILDINPROCEDURE_H

#include "MgInterpreter.h"

typedef struct MgBuildInProcedure MgBuildInProcedure;

typedef MgStatus* (*MgBuildInProcedure_Func)(MgInterpreter* interpreter,
					     MgObject* arg,
					     MgObject** output);

MgStatus* MgBuildInProcedure_create(MgBuildInProcedure_Func func,
				    MgBuildInProcedure** procedure);

#endif /* MGBUILDINPROCEDURE_H */
