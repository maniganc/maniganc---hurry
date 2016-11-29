
#include "MgBuildInProcedure.h"

#define CAT2(x, y) x##y
#define MG_BUILDIN_PROCEDURE_NAME(f) CAT2(mg_buildin_procedure_,f)
#define MG_BUILDIN_PROCEDURE(f, identifier)                             \
  MgStatus* MG_BUILDIN_PROCEDURE_NAME(f)(MgObject* arg,                 \
                                         MgObject** output,             \
                                         MgInterpreter* interpreter,    \
                                         MgEnv* env)


typedef struct Mg_buildin_procedure {
  MgStatus* (*func)(MgObject* arg,MgObject** output,
                    MgInterpreter* interpreter,
                    MgEnv* env);
  const char* name;
}Mg_buildin_procedure;
