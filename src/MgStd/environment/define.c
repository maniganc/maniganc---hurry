
#include "buildin_procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"

static const MgStatus error_not_an_identifier = {
  .message = "define require an identifier"
};

static const MgStatus error_no_object = {
  .message = "define require an identifier and an object to bind"
};

static const MgStatus error_too_much_args = {
  .message = "define require only an identifier and an object to bind"
};

MG_BUILDIN_PROCEDURE(define, "define") {
  MgStatus* status;

  if ((MgList*)arg == MgInterpreter_get_emptylist(interpreter)) {
    return &error_not_an_identifier;
  }
  
  /* get car, must be an identifier for now */
  MgObject* car = MgList_get_car((MgList*)arg);
  if (!Mg_is_an_identifier(car)) return &error_not_an_identifier;
  MgIdentifier* id = (MgIdentifier*)car;

  /* then cdar can be anything */
  MgList* obj_list = (MgList*)MgList_get_cdr((MgList*)arg);
  if (obj_list == MgInterpreter_get_emptylist(interpreter)) {
    return &error_no_object;
  }
  
  MgObject* obj = MgList_get_car(obj_list);

  /* and cddar is () : not other argument is allowed */
  if (MgList_get_cdr(obj_list) != (MgObject*)Mg_emptyList) {
    return &error_too_much_args;
  }
  
  /* evaluate it */
  MgObject* obj_eval;
  status = MgObject_evaluate(obj, &obj_eval, interpreter, env);
  if (status != Mg_ok) goto error;
  /* use it for a while */
  MgObject_add_reference(obj_eval);

  /* add a bond between the two of them in the current env */
  status =  MgEnv_add_identifier(&env, id, obj_eval, 1/* limit scope*/);
  if (status != Mg_ok) goto drop_and_error;

  /* now obj_eval is useless here */
  MgObject_drop_reference(obj_eval);

  /* return identifier */
  *output = (MgObject*)id;
  return Mg_ok;

  drop_and_error:
  MgObject_drop_reference(obj_eval);
  error:
  return status;
}
