

#include "buildin_procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"
#include "MgBool.h"
#include "MgObjectReference.h"
#include "MgString.h"

MG_BUILDIN_PROCEDURE(is_symbol, "symbol?") {

  static const MgStatus error_params = {
    .message = "symbol? require one argument"
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

  if (Mg_is_an_identifier(obj_eval)) {
    *output = (MgObject*)MgBool_true;
  }
  else {
    *output = (MgObject*)MgBool_false;
  }

  MgObject_drop_reference(obj_eval);

  return Mg_ok;
}



MG_BUILDIN_PROCEDURE(identifier_to_string, "symbol->string") {

  static const MgStatus error_params = {
    .message = "symbol->string require a symbol"
  };

  MgStatus* s = &error_params;

  /* one arg only */
  if ((MgList*)arg == Mg_emptyList) goto error;
  if ((MgList*)MgList_get_cdr((MgList*)arg) != Mg_emptyList) goto error;

  MgObject* identifier_ref = MgList_get_car((MgList*)arg);

  MgIdentifier* identifier_eval;
  MgObject_evaluate(identifier_ref, (MgObject**)&identifier_eval, interpreter, env);

  MgObject_add_reference((MgObject*)identifier_eval);

  if (!Mg_is_an_identifier((MgObject*)identifier_eval)) goto drop_eval_and_other_and_error;

  const char* name = MgIdentifier_get_name(identifier_eval);

  MgString* str;
  s = MgString_create_from_string(&str, name);
  if ( s != Mg_ok ) goto drop_eval_and_other_and_error;

  MgObject_drop_reference((MgObject*)identifier_eval);

  *output = (MgObject*)str;

  return Mg_ok;

  drop_eval_and_other_and_error:
  MgObject_drop_reference((MgObject*)identifier_eval);
  error:
  return s;
}
