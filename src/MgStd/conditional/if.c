
#include "buildin_procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"
#include "MgBool.h"
#include "MgObjectReference.h"
#include "MgUnspecified.h"

static const MgStatus error_params = {
  .message = "if require a conditional and two cases "
};

MG_BUILDIN_PROCEDURE(if, "if") {
  MgStatus* status;

  /* check for conditional */
  if ((MgList*)arg == Mg_emptyList) {
    return &error_params;
  }
  MgObject* cond = MgList_get_car((MgList*)arg);

  /* check for true case */
  arg = MgList_get_cdr((MgList*)arg);
  if ((MgList*)arg == Mg_emptyList) {
    return &error_params;
  }
  MgObject* true_case = MgList_get_car((MgList*)arg);

  /* check for false case */
  MgObject* false_case;
  arg = MgList_get_cdr((MgList*)arg);
  if ((MgList*)arg != Mg_emptyList) {
    false_case = MgList_get_car((MgList*)arg);
    /* check ending */
    arg = MgList_get_cdr((MgList*)arg);
    if ( arg != (MgObject*)Mg_emptyList ) return &error_params;
  }
  else {
    false_case = (MgObject*)Mg_unspecified;
  }

  /* evaluate conditional */
  MgObject* cond_evaluated;
  status = MgObject_evaluate(cond, &cond_evaluated, interpreter, env);
  if (status != Mg_ok) return status;
  
  /* use it for a while */
  MgObject_add_reference(cond_evaluated);

  /* get case according to the result of the conditional */
  MgObject* case_to_evaluate;
  if (Mg_is_false(cond_evaluated)) {
    case_to_evaluate = false_case;
  }
  else {
    case_to_evaluate = true_case;
  }

  /* now cond_evaluated is useless */
  MgObject_drop_reference(cond_evaluated);

  /* return a ref on the choosen case (tail call optimisation) */
  return MgObjectReference_return_ref(case_to_evaluate,
				      (MgObjectReference**)output,
				      interpreter, env);
}
