#ifndef PROCEDURE_MACRO_H_
#define PROCEDURE_MACRO_H_

#include "MgProcedure.h"

#define LINK(x, y) x##y
#define MG_PROCEDURE_NAME(f) LINK(mg_procedure_,f)
#define MG_PROCEDURE(f, identifier)                                     \
  MgStatus* MG_PROCEDURE_NAME(f)(MgObject* arg,                         \
                                         MgObject** output,             \
                                         MgInterpreter* interpreter,    \
                                         MgEnv* env)

typedef struct Mg_procedure {
  MgStatus* (*func)(MgObject* arg,
                    MgObject** output,
                    MgInterpreter* interpreter,
                    MgEnv* env);
  const char* name;
}Mg_procedure;

#endif /* PROCEDURE_MACRO_H_ */