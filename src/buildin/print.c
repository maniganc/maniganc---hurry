
#include "buildin_procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"
#include "MgInteger.h"

MG_BUILDIN_PROCEDURE(pp, "write") {
  MgStatus* status;
  
  if ((MgList*)arg == MgInterpreter_get_emptylist(interpreter)) {
    *output = arg;
    return Mg_ok;
  }

  MgObject* obj = MgList_get_car((MgList*)arg);

  MgObject* obj_evaluated;
  status = MgObject_evaluate(obj, &obj_evaluated, interpreter, env);
  if (status != Mg_ok) return status;

  MgObject_add_reference(obj_evaluated);
  
  MgObject_represent(obj_evaluated, stdout);
  fprintf(stdout, "\n");

  MgObject_drop_reference(obj_evaluated);

  *output = (MgObject*)MgInterpreter_get_emptylist(interpreter);
  
  return Mg_ok;
}
