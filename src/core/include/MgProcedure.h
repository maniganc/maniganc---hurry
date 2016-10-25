#ifndef MGPROCEDURE_H_
#define MGPROCEDURE_H_

#include "MgObject.h"
#include "MgList.h"

typedef struct MgProcedure MgProcedure;

typedef MgStatus* (* MgProcedure_Global_Func ) (MgObject * arg,
                                MgObject** output,
                                MgInterpreter* interpreter,
                                MgEnv* env);

MgStatus* MgProcedure_create(MgProcedure** procedure,
                              MgProcedure_Global_Func func);

MgStatus* MgProcedure_destroy(MgProcedure* procedure);

#endif /* MGBUILDINPRIMITIVES_H_ */