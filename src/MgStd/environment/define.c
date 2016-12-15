
#include "buildin_procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"

#include "MgProcedure.h"


static const MgStatus error_no_object = {
  .message = "define require an identifier and an object to bind"
};

static const MgStatus error_too_much_args = {
  .message = "define require only an identifier and an object to bind"
};

MG_BUILDIN_PROCEDURE(define, "define") {
  MgStatus* status;

  if ((MgList*)arg == Mg_emptyList) {
    static const MgStatus error = {
      .message = "define needs arguments"};
    return &error;
  }

  /* get car, must be an identifier for symbolic binding */
  MgObject* car = MgList_get_car((MgList*)arg);
  if (!Mg_is_an_identifier(car)) goto define_not_an_identifier;
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

  /* now obj_eval is useless here */
  MgObject_drop_reference(obj_eval);

  if (status != Mg_ok) goto error;

  /* return identifier */
  *output = (MgObject*)id;
  return Mg_ok;

  error:
  return status;



  define_not_an_identifier: {
    /* could be a procedure declaration */
    /* first arg : (proc_name arg1 arg2 ...) */
    MgList* arg1 = (MgList*)MgList_get_car((MgList*)arg);
    if (!Mg_is_a_list((MgObject*)arg1)) {
      static const MgStatus error = {
        .message = "unknown invocation of define "};
      return &error;
    }

    /* check format */
    /* check identifier */
    MgIdentifier* proc_id = (MgIdentifier*)MgList_get_car(arg1);
    if ( !Mg_is_an_identifier((MgObject*)proc_id) ) {
      static const MgStatus error = {
        .message = "define: procedure declaration needs an identifier"};
      status = &error;
      goto error;
    }
    
    /* check args */
    MgList* proc_args = (MgList*)MgList_get_cdr((MgList*)arg1);
    /* check if every argument is an identifier */
    MgList* proc_args_iter = proc_args;
    while ( proc_args_iter != Mg_emptyList ) {
      MgObject* argument = MgList_get_car(proc_args_iter);
      if ( !Mg_is_an_identifier(argument) ) {
        static MgStatus error =  { "define: argument is not an identifier" };
        status = &error;
        goto error;
      }
      proc_args_iter = (MgList*)MgList_get_cdr(proc_args_iter);
    }

    /* following args, code : exp1 exp2 ... */
    arg = MgList_get_cdr((MgList*)arg);
    if ( (MgList*)arg == Mg_emptyList ) {
      static const MgStatus error = {
        .message = "define: procedure declaration needs code to execute"};
      status = &error;
      goto error;
    }
    MgList* code = (MgList*)arg;
    
    /* create procedure */
    MgProcedure* proc;
    status = MgProcedure_create(&proc, proc_args, code, env);
    if ( status != Mg_ok ) goto error;
    
    MgObject_add_reference((MgObject*)proc);
    
    /* bind procedure to identifier */
    status = MgEnv_add_identifier(&env, proc_id, (MgObject*)proc, 1 /* scope limited */);
    if ( status != Mg_ok ) {
      MgObject_drop_reference((MgObject*)proc);
      goto error;
    }

    MgObject_drop_reference((MgObject*)proc);

    *output = (MgObject*)proc_id;
    
    return Mg_ok;
  }
}
