

#include "buildin_procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"
#include "MgBool.h"
#include "MgObjectReference.h"
#include "MgInteger.h"
#include "MgChar.h"

static const MgStatus error_params = {
  .message = "integer? require one argument"
};

MG_BUILDIN_PROCEDURE(is_integer, "integer?") {

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

  if (Mg_is_an_integer(obj_eval)) {
    *output = (MgObject*)MgBool_true;
  }
  else {
    *output = (MgObject*)MgBool_false;
  }

  MgObject_drop_reference(obj_eval);

  return Mg_ok;
}


MG_BUILDIN_PROCEDURE(integer_to_char, "integer->char") {

  static const MgStatus error_params = {
    .message = "integer->char require a integer between 0 and 255"
  };

  MgStatus* s = &error_params;

  /* one arg only */
  if ((MgList*)arg == Mg_emptyList) goto error;
  if ((MgList*)MgList_get_cdr((MgList*)arg) != Mg_emptyList) goto error;

  MgObject* integer_ref = MgList_get_car((MgList*)arg);
  
  MgInteger* integer_eval;
  MgObject_evaluate(integer_ref, (MgObject**)&integer_eval, interpreter, env);

  MgObject_add_reference((MgObject*)integer_eval);

  if (!Mg_is_an_integer((MgObject*)integer_eval)) goto drop_eval_and_other_and_error;

  int val = MgInteger_get_value(integer_eval);
  
  if ( val < 0 || val > 255 ) goto drop_eval_and_other_and_error;

  MgChar* ch;
  s = MgChar_create(&ch, val);
  if ( s != Mg_ok ) goto drop_eval_and_other_and_error;

  MgObject_drop_reference((MgObject*)integer_eval);

  *output = (MgObject*)ch;

  return Mg_ok;

  drop_eval_and_other_and_error:
  MgObject_drop_reference((MgObject*)integer_eval);
  error:
  return s;
}
