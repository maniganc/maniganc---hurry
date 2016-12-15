
#include "buildin_procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"

#include "MgProcedure.h"

MG_BUILDIN_PROCEDURE(lambda, "lambda") {
  MgStatus* status;

  if ( (MgList*)arg == Mg_emptyList ) {
    static MgStatus error =  { "lambda require a list of arguments"
                               " followed by a sequence of expressions to execute" };
    status = &error;
    goto error;
  }

  /* get procedure arguments */
  MgList* proc_args = (MgList*)MgList_get_car((MgList*)arg);
  /* check if every argument is an identifier */
  MgList* proc_args_iter = proc_args;
  while ( proc_args_iter != Mg_emptyList ) {
    MgObject* argument = MgList_get_car(proc_args_iter);
    if (!Mg_is_an_identifier(argument)) {
      static MgStatus error =  { "lambda: argument is not an identifier" };
      status = &error;
      goto error;
    }
    proc_args_iter = (MgList*)MgList_get_cdr(proc_args_iter);
  }

  /* get code */
  arg = MgList_get_cdr((MgList*)arg);
  MgList* code = (MgList*)arg;

  return MgProcedure_create((MgProcedure**)output, proc_args, code, env);
  
  error:
  return status;
}
