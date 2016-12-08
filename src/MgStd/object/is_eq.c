

#include "buildin_procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"
#include "MgBool.h"
#include "MgObjectReference.h"
#include "MgString.h"
#include "MgChar.h"

MG_BUILDIN_PROCEDURE(is_eq, "eq?") {

  MgStatus* s;

  MgObject* first_object = NULL;

  while ( arg != (MgObject*)Mg_emptyList ) {
    /* get next arg, evaluate it */
    MgObject* arg_obj = MgList_get_car((MgList*)arg);
    MgObject* arg_obj_eval;
    s = MgObject_evaluate(arg_obj, &arg_obj_eval, interpreter, env);
    if ( s != Mg_ok ) return s;

    MgObject_add_reference(arg_obj_eval);
    MgObject_drop_reference(arg_obj_eval);

    if ( first_object == NULL ) {
      first_object = arg_obj_eval;
    }
    else {
      MgObject* next_object = arg_obj_eval;
      if ( next_object != first_object ) {
        *output = (MgObject*)MgBool_false;
        return Mg_ok;
      }
    }

    /* next arg */
    arg = MgList_get_cdr((MgList*)arg);
  };

  *output = (MgObject*)MgBool_true;
  return Mg_ok;
}
