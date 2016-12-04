
#include "buildin_procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"

static const MgStatus error_args = {
  .message = "eval require only an object and optionally an environment"
};

MG_BUILDIN_PROCEDURE(eval, "eval") {
  MgStatus* s;

  if ((MgList*)arg == MgInterpreter_get_emptylist(interpreter)) {
    return &error_args;
  }

  /* get car, must be an object to evaluate*/
  MgObject* obj = MgList_get_car((MgList*)arg);

  /* evaluate object, in current env*/
  MgObject* obj_eval;
  s = MgObject_evaluate((MgObject*)obj, (MgObject**)&obj_eval, interpreter, env);
  if (s != Mg_ok) goto error;
  
  /* cdar can be optionally an environment (at least a list) */
  MgList* arg_2 = (MgList*)MgList_get_cdr((MgList*)arg);
  if (arg_2 != Mg_emptyList) {
    /* optional arg must be a list/env */
    MgList* new_env = (MgList*)MgList_get_car(arg_2);
    /* evaluate it  */
    MgList* new_env_eval;
    s = MgObject_evaluate((MgObject*)new_env, (MgObject**)&new_env_eval, interpreter, env);
    if (s != Mg_ok) goto drop_obj_eval_and_error;
    /* check if it's a list */
    if (!Mg_is_a_list((MgObject*)new_env_eval)) {
      return &error_args;
    }
    /* redefine env */
    env = new_env_eval;

    /* and then cddr is () : not other argument is allowed */
    MgList* arg_3 = (MgList*)MgList_get_cdr((MgList*)arg_2);
    if (arg_3 != Mg_emptyList) {
      return &error_args;
    }
  }

  /* return object reference of evaluation, in the given environment */
  return MgObjectReference_return_ref(obj_eval, (MgObjectReference**)output,
                                      interpreter, env);

  drop_obj_eval_and_error:
  MgObject_add_reference(obj_eval);
  MgObject_drop_reference(obj_eval);
  error:
  return s;
}
