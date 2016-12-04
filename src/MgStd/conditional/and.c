
#include "buildin_procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"
#include "MgBool.h"
#include "MgObjectReference.h"


MG_BUILDIN_PROCEDURE(and, "and") {
  /* check if there is no arg */
  if ((MgList*)arg == Mg_emptyList) {
    *output = (MgObject*)MgBool_true;
    return Mg_ok;
  }

  MgObject* returned_obj = (MgObject*)MgBool_false;

  /* iterate over args */
  MgObject* obj;
  do {
    /* get next object */
    obj = MgList_get_car((MgList*)arg);

    /* if it is the last object, we don't need to kwow its evaluation */
    if (MgList_get_cdr((MgList*)arg) == (MgObject*)Mg_emptyList) {
      return MgObjectReference_return_ref(obj,
                                          (MgObjectReference**)output,
                                          interpreter, env);
    }

    /* evaluate it */
    MgObject* obj_eval;
    MgStatus* s = MgObject_evaluate(obj, &obj_eval, interpreter, env);

    /* check errors */
    if (s != Mg_ok) {
      return s;
    }

    /* exit with false on false */
    if (obj_eval == (MgObject*)MgBool_false) {
      returned_obj = obj_eval;
      break;
    }

    /* we used the object
     * ensure that the object get cleaned if it have to */
    MgObject_add_reference(obj_eval);
    MgObject_drop_reference(obj_eval);

    /* next arg */
    arg = MgList_get_cdr((MgList*)arg);
  } while(1);

  *output = (MgObject*)returned_obj;

  return Mg_ok;
}
