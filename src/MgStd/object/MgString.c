

#include "buildin_procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"
#include "MgBool.h"
#include "MgObjectReference.h"
#include "MgString.h"
#include "MgChar.h"

static const MgStatus error_params = {
  .message = "string? require one argument"
};

MG_BUILDIN_PROCEDURE(is_string, "string?") {

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

  if (Mg_is_a_string(obj_eval)) {
    *output = (MgObject*)MgBool_true;
  }
  else {
    *output = (MgObject*)MgBool_false;
  }

  MgObject_drop_reference(obj_eval);

  return Mg_ok;
}


static const MgStatus error_params_string_to_symbol = {
  .message = "string->symbol require a symbol"
};


MG_BUILDIN_PROCEDURE(string_to_symbol, "string->symbol") {
  MgStatus* s = &error_params_string_to_symbol;

  /* one arg only */
  if ((MgList*)arg == Mg_emptyList) goto error;
  if ((MgList*)MgList_get_cdr((MgList*)arg) != Mg_emptyList) goto error;

  MgObject* string_ref = MgList_get_car((MgList*)arg);

  MgString* string_eval;
  s = MgObject_evaluate(string_ref, (MgObject**)&string_eval, interpreter, env);
  if ( s != Mg_ok ) goto error;
  
  MgObject_add_reference((MgObject*)string_eval);

  /* check if it is really a string */
  s = &error_params_string_to_symbol;
  if (!Mg_is_a_string((MgObject*)string_eval)) goto drop_eval_and_other_and_error;

  MgIdentifier* id;
  s = MgIdentifier_create_from_string(&id, MgString_get_string(string_eval));
  if (s != Mg_ok) goto drop_eval_and_other_and_error;

  MgObject_drop_reference((MgObject*)string_eval);

  *output = (MgObject*)id;

  return Mg_ok;

  drop_eval_and_other_and_error:
  MgObject_drop_reference((MgObject*)string_eval);
  error:
  return s;
}


static const MgStatus error_params_string_to_list = {
  .message = "string->list require a string"
};



MG_BUILDIN_PROCEDURE(string_to_list, "string->list") {
  MgStatus* s = &error_params_string_to_list;

  /* one arg only */
  if ((MgList*)arg == Mg_emptyList) goto error;
  if ((MgList*)MgList_get_cdr((MgList*)arg) != Mg_emptyList) goto error;

  MgObject* string_ref = MgList_get_car((MgList*)arg);

  MgString* string_eval;
  s = MgObject_evaluate(string_ref, (MgObject**)&string_eval, interpreter, env);
  if ( s != Mg_ok ) goto error;

  MgObject_add_reference((MgObject*)string_eval);

  s = &error_params_string_to_list;
  if (!Mg_is_a_string((MgObject*)string_eval)) goto drop_eval_and_other_and_error;

  MgList* list;
  s = MgList_create(&list);
  if ( s != Mg_ok ) goto drop_eval_and_other_and_error;

  const char* str =  MgString_get_string(string_eval);

  MgList* list_tail = list;
  while(*str) {
    MgChar* ch;
    s = MgChar_create(&ch, *str);
    if ( s != Mg_ok) goto drop_eval_and_other_and_error;


    /* at the beginning, list is empty.
     * the first push_back creates the head of the list
     * list should point to the head of this list */
    if (MgList_is_empty(list_tail)) {
      s = MgList_push_back(&list_tail, (MgObject*)ch);
      list = list_tail;
    }
    else {
      s = MgList_push_back(&list_tail, (MgObject*)ch);
    }
    
    if ( s != Mg_ok) {
      MgObject_destroy((MgObject*)ch);
      goto drop_eval_and_other_and_error;
    }

    str++;
  }

  MgObject_drop_reference((MgObject*)string_eval);

  *output = (MgObject*)list;

  return Mg_ok;

  drop_eval_and_other_and_error:
  MgObject_drop_reference((MgObject*)string_eval);
  error:
  return s;
}
