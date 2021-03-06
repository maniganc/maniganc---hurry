

#include "buildin_procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"
#include "MgBool.h"
#include "MgObjectReference.h"
#include "MgChar.h"
#include "MgString.h"
#include "MgInteger.h"

MG_BUILDIN_PROCEDURE(is_char, "char?") {

  static const MgStatus error_params = {
    .message = "char? require one argument"
  };

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

  if (Mg_is_a_char(obj_eval)) {
    *output = (MgObject*)MgBool_true;
  }
  else {
    *output = (MgObject*)MgBool_false;
  }

  MgObject_drop_reference(obj_eval);

  return Mg_ok;
}



MG_BUILDIN_PROCEDURE(char_to_string, "char->string") {

  static const MgStatus error_params = {
    .message = "char->string require a char"
  };

  MgStatus* s = &error_params;

  /* one arg only */
  if ((MgList*)arg == Mg_emptyList) goto error;
  if ((MgList*)MgList_get_cdr((MgList*)arg) != Mg_emptyList) goto error;

  MgObject* char_ref = MgList_get_car((MgList*)arg);

  MgChar* char_eval;
  s = MgObject_evaluate(char_ref, (MgObject**)&char_eval, interpreter, env);
  if ( s != Mg_ok ) goto error;

  MgObject_add_reference((MgObject*)char_eval);

  s = &error_params;
  if (!Mg_is_a_char((MgObject*)char_eval)) goto drop_eval_and_other_and_error;

  char ch[2] = {MgChar_get_char(char_eval), 0};
  
  MgString* str;
  s = MgString_create_from_string(&str, ch);
  if ( s != Mg_ok ) goto drop_eval_and_other_and_error;

  MgObject_drop_reference((MgObject*)char_eval);

  *output = (MgObject*)str;

  return Mg_ok;

  drop_eval_and_other_and_error:
  MgObject_drop_reference((MgObject*)char_eval);
  error:
  return s;
}



MG_BUILDIN_PROCEDURE(char_to_integer, "char->integer") {

  static const MgStatus error_params = {
    .message = "char->integer require a char"
  };

  MgStatus* s = &error_params;

  /* one arg only */
  if ((MgList*)arg == Mg_emptyList) goto error;
  if ((MgList*)MgList_get_cdr((MgList*)arg) != Mg_emptyList) goto error;

  MgObject* char_ref = MgList_get_car((MgList*)arg);

  MgChar* char_eval;
  s = MgObject_evaluate(char_ref, (MgObject**)&char_eval, interpreter, env);
  if ( s != Mg_ok ) goto error;

  MgObject_add_reference((MgObject*)char_eval);

  s = &error_params;
  if (!Mg_is_a_char((MgObject*)char_eval)) goto drop_eval_and_other_and_error;

  char ch = MgChar_get_char(char_eval);

  MgInteger* in;
  s = MgInteger_create_from_int(&in, ch);
  if ( s != Mg_ok ) goto drop_eval_and_other_and_error;

  MgObject_drop_reference((MgObject*)char_eval);

  *output = (MgObject*)in;

  return Mg_ok;

  drop_eval_and_other_and_error:
  MgObject_drop_reference((MgObject*)char_eval);
  error:
  return s;
}
