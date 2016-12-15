
#include "buildin_procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"
#include "MgBool.h"

MG_BUILDIN_PROCEDURE(begin, "begin") {
  return MgList_evaluate_sequential((MgList*)arg, output, interpreter, env);
}
