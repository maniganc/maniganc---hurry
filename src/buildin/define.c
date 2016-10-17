
#include "buildin_procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgEnvironment.h"


static const MgStatus error_not_an_identifier = {
  .message = "define require an identifier"
};


MG_BUILDIN_PROCEDURE(define, "define") {
  MgStatus* status;

  /* get car, must be an identifier for now */
  MgObject* car = MgList_get_car((MgList*)arg);
  if (!Mg_is_an_identifier(car)) return &error_not_an_identifier;
  MgIdentifier* id = (MgIdentifier*)car;

  /* then cdar can be anything */
  MgObject* obj = MgList_get_car((MgList*)MgList_get_cdr((MgList*)arg));

  /* evaluate it */
  MgObject* obj_eval;
  status = MgObject_evaluate(obj, &obj_eval, interpreter, env);
  if (status != Mg_ok) goto error;
  /* use it for a while */
  MgObject_add_reference(obj_eval);

  /* add a bond between the two of them in the current env */
  status =  MgEnv_add_identifier(&env, id, obj);
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
