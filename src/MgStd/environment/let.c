
#include "buildin_procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"

#include "MgProcedure.h"

MG_BUILDIN_PROCEDURE(let, "let") {
  MgStatus* status;

  static MgStatus error_syntax =
    { "let require a list of lists with arguments and values,"
      " followed by a sequence of expressions to execute" };

  if ( (MgList*)arg == Mg_emptyList ) {
    return &error_syntax;
  }

  /* get init values */
  MgList* init_args = (MgList*)MgList_get_car((MgList*)arg);
  if (!Mg_is_a_list((MgObject*)init_args)) {
    return &error_syntax;
  }

  /* get code */
  arg = MgList_get_cdr((MgList*)arg);
  MgList* code = (MgList*)arg;


  /* create env */
  MgEnv* let_env;
  status = MgEnv_create(&let_env, env);
  if ( status != Mg_ok ) return status;

  MgObject_add_reference((MgObject*)let_env);

  /* check if every argument is an identifier
   * fill env with init values */
  MgObject* val;
  MgList* let_args_iter = init_args;
  while ( let_args_iter != Mg_emptyList ) {
    /* get a bond */
    MgList* bond = (MgList*)MgList_get_car(let_args_iter);
    status = &error_syntax;
    if ( !Mg_is_a_list((MgObject*)bond) ) goto drop_env_and_error;
    if ( bond == Mg_emptyList ) goto drop_env_and_error;
    
    /* get id  */
    MgIdentifier* id = (MgIdentifier*)MgList_get_car(bond);
    
    /* get value */
    bond = (MgList*)MgList_get_cdr(bond);
    if ( !Mg_is_a_list((MgObject*)bond) ) goto drop_env_and_error;
    if ( bond == Mg_emptyList ) goto drop_env_and_error;
    /* eval value */
    MgObject* val_ref = MgList_get_car(bond);
    status = MgObject_evaluate(val_ref, &val, interpreter, env);
    if ( status != Mg_ok ) goto drop_env_and_error;

    MgObject_add_reference(val);
    
    /* check end */
    status = &error_syntax;
    if ( MgList_get_cdr(bond) != (MgObject*)Mg_emptyList )
      goto drop_eval_and_other_and_error;

    /* store in let env */
    status = MgEnv_add_identifier(&let_env, id, val_ref, 1/* scope limited */);
    if ( status != Mg_ok ) goto drop_eval_and_other_and_error;

    MgObject_drop_reference(val);

    /* next */
    let_args_iter = (MgList*)MgList_get_cdr(let_args_iter);
  }

  status =  MgList_evaluate_sequential(code, output, interpreter, let_env);

  MgObject_drop_reference((MgObject*)let_env);
  
  return status;

  drop_eval_and_other_and_error:
  MgObject_drop_reference(val);
  drop_env_and_error:
  MgObject_drop_reference((MgObject*)let_env);
  error:
  return status;
}
