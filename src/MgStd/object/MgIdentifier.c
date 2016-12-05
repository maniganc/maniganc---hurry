

#include "buildin_procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"
#include "MgBool.h"
#include "MgObjectReference.h"

static const MgStatus error_params = {
  .message = "symbol? require one argument"
};

MG_BUILDIN_PROCEDURE(is_symbol, "symbol?") {

  /* check for arg */
  if ((MgList*)arg == Mg_emptyList) {
    return &error_params;
  }

  if ((MgList*)MgList_get_cdr((MgList*)arg) != Mg_emptyList) {
    return &error_params;
  }

  /* evaluate arg */
  MgObject* obj = MgList_get_car((MgList*)arg);
  MgObject* obj_eval;
  MgStatus* s = MgObject_evaluate(obj, &obj_eval, interpreter, env);
  if (s != Mg_ok) return s;

  MgObject_add_reference(obj_eval);

  if (Mg_is_an_identifier(obj_eval)) {
    *output = (MgObject*)MgBool_true;
  }
  else {
    *output = (MgObject*)MgBool_false;
  }

  MgObject_drop_reference(obj_eval);

  return Mg_ok;
}
