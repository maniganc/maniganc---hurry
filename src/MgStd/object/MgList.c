

#include "buildin_procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"
#include "MgBool.h"
#include "MgObjectReference.h"
#include "MgString.h"
#include "MgChar.h"
#include "MgPair.h"

/* predicates */

MG_BUILDIN_PROCEDURE(is_null, "null?") {

  static const MgStatus error_params = {
    .message = "null? require one argument"
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

  if (obj_eval == (MgObject*)Mg_emptyList) {
    *output = (MgObject*)MgBool_true;
  }
  else {
    *output = (MgObject*)MgBool_false;
  }

  MgObject_drop_reference(obj_eval);

  return Mg_ok;
}


MG_BUILDIN_PROCEDURE(is_list, "list?") {

  static const MgStatus error_params_list_p = {
    .message = "list? require one argument"
  };

  /* check for arg */
  if ((MgList*)arg == Mg_emptyList) {
    return &error_params_list_p;
  }

  if ((MgList*)MgList_get_cdr((MgList*)arg) != Mg_emptyList) {
    return &error_params_list_p;
  }

  /* evaluate arg */
  MgObject* obj = MgList_get_car((MgList*)arg);
  MgObject* obj_eval;
  MgStatus* s = MgObject_evaluate(obj, &obj_eval, interpreter, env);
  if (s != Mg_ok) return s;

  MgObject_add_reference(obj_eval);

  if (Mg_is_a_list(obj_eval)) {
    *output = (MgObject*)MgBool_true;
  }
  else {
    *output = (MgObject*)MgBool_false;
  }

  MgObject_drop_reference(obj_eval);

  return Mg_ok;
}

MG_BUILDIN_PROCEDURE(is_pair, "pair?") {

  static const MgStatus error_params_pair_p = {
    .message = "pair? require one argument"
  };

  /* check for arg */
  if ((MgList*)arg == Mg_emptyList) {
    return &error_params_pair_p;
  }

  if ((MgList*)MgList_get_cdr((MgList*)arg) != Mg_emptyList) {
    return &error_params_pair_p;
  }

  /* evaluate arg */
  MgObject* obj = MgList_get_car((MgList*)arg);
  MgObject* obj_eval;
  MgStatus* s = MgObject_evaluate(obj, &obj_eval, interpreter, env);
  if (s != Mg_ok) return s;

  MgObject_add_reference(obj_eval);

  if (Mg_is_a_list(obj_eval) &&
      (MgList*)MgList_get_cdr((MgList*)obj_eval) != Mg_emptyList) {
    *output = (MgObject*)MgBool_true;
  }
  else {
    *output = (MgObject*)MgBool_false;
  }

  MgObject_drop_reference(obj_eval);

  return Mg_ok;
}

/* conversions */


MG_BUILDIN_PROCEDURE(list_to_string, "list->string") {

  static const MgStatus error_params_list_to_string = {
    .message = "list->string require one proper list of char"
  };

  MgStatus* s = &error_params_list_to_string;

  /* one arg only */
  if ((MgList*)arg == Mg_emptyList) goto error;
  if ((MgList*)MgList_get_cdr((MgList*)arg) != Mg_emptyList) goto error;

  MgObject* list_ref = MgList_get_car((MgList*)arg);

  MgList* list_eval;
  s = MgObject_evaluate(list_ref, (MgObject**)&list_eval, interpreter, env);
  if ( s != Mg_ok ) goto error;

  MgObject_add_reference((MgObject*)list_eval);

  /* check if it is really a list */
  s = &error_params_list_to_string;
  if (!Mg_is_a_list((MgObject*)list_eval)) {
    MgObject_drop_reference((MgObject*)list_eval);
    goto error;
  }

  /* create a string */
  vector_char str;
  vector_char_init(&str);

  /* iterate over args */
  MgList* sublist = list_eval;
  while(sublist != Mg_emptyList) {
    if (!Mg_is_a_list((MgObject*)sublist)) goto drop_eval_and_other_and_error;

    /* get next object */
    MgObject* obj = MgList_get_car((MgList*)sublist);

    /* check if it's a char */
    if (!Mg_is_a_char(obj)) goto drop_eval_and_other_and_error;

    vector_char_push(&str, MgChar_get_char((MgChar*)obj));

    /* next arg */
    sublist = (MgList*)MgList_get_cdr((MgList*)sublist);
  }

  /* eval is no longer needed */
  MgObject_drop_reference((MgObject*)list_eval);

  /* push ending \0 */
  vector_char_push(&str, '\0');

  /* create string object */
  MgString* mgstring;
  s = MgString_create_from_string(&mgstring, vector_char_get_raw_ptr(&str));
  if (s != Mg_ok) goto free_vector_and_error;

  /* destroy vector */
  vector_char_deinit(&str);

  *output = (MgObject*)mgstring;

  return Mg_ok;

  drop_eval_and_other_and_error:
  MgObject_drop_reference((MgObject*)list_eval);
  free_vector_and_error:
  vector_char_deinit(&str);
  error:
  return s;
}

MG_BUILDIN_PROCEDURE(cons, "cons") {

  static const MgStatus error_params = {
    .message = "cons require two arguments"
  };

  MgStatus* s;

  /* require two args */
  s = &error_params;
  if ( arg == (MgObject*)Mg_emptyList ) goto error;
  MgObject* arg1 = MgList_get_car((MgList*)arg);

  arg = MgList_get_cdr((MgList*)arg);
  if ( arg == (MgObject*)Mg_emptyList ) goto error;
  MgObject* arg2 = MgList_get_car((MgList*)arg);

  arg = MgList_get_cdr((MgList*)arg);
  if ( arg != (MgObject*)Mg_emptyList ) goto error;

  /* evaluate args */
  MgObject* arg1_eval;
  s = MgObject_evaluate(arg1, &arg1_eval, interpreter, env);
  if ( s != Mg_ok ) goto error;
  MgObject_add_reference(arg1_eval);

  MgObject* arg2_eval;
  s = MgObject_evaluate(arg2, &arg2_eval, interpreter, env);
  if ( s != Mg_ok ) goto drop_arg1_and_error;
  MgObject_add_reference(arg2_eval);

  /* return pair */
  MgPair* pair;
  s = MgPair_create(&pair, arg1_eval, arg2_eval);
  if ( s != Mg_ok ) goto drop_arg2_and_error;

  *output = (MgObject*)pair;
  return Mg_ok;

  drop_arg2_and_error:
  MgObject_drop_reference(arg2_eval);
  drop_arg1_and_error:
  MgObject_drop_reference(arg1_eval);
  error:
  return s;
}
