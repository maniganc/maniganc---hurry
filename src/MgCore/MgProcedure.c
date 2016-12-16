
#include "MgProcedure.h"

#include <stdlib.h>

struct MgProcedure { MgObject base;
  MgList* argument;
  MgList* code;
  MgEnv* environment;
};

static MgStatus* evaluate(MgProcedure* proc, MgObject** output,
                          MgInterpreter* interpreter,
                          MgEnv* env) {
  *output = (MgObject*)proc;
  return Mg_ok;
}

static MgStatus* evaluate_on(MgProcedure* proc,
                             MgObject* target,
                             MgObject** output,
                             MgInterpreter* interpreter,
                             MgEnv* env) {
  MgStatus* s;

  MgList* args = proc->argument;
  MgList* vals = (MgList*)target;
  while ( args != Mg_emptyList ) {

    /* check if there is a value associated with the argument */
    if ( vals == Mg_emptyList ) {
      static const MgStatus error_invalid_number_of_args =
	{ "procedure: too few arguments provided" };
      s = &error_invalid_number_of_args;
      goto error;
    }

    /* evaluate value in current env */
    MgObject* value = MgList_get_car(vals);
    MgObject* value_eval;
    s = MgObject_evaluate(value, &value_eval, interpreter, env);
    if ( s != Mg_ok ) goto error;

    MgObject_add_reference(value_eval);
    
    /* assign value to symbol */
    MgIdentifier* id = (MgIdentifier*)MgList_get_car(args);
    s = MgEnv_add_identifier(&proc->environment, id, value_eval, 1/* scope limited*/);
    
    MgObject_drop_reference(value_eval);

    if ( s != Mg_ok ) goto error;

    /* next args & value */
    args = (MgList*)MgList_get_cdr(args);
    vals = (MgList*)MgList_get_cdr(vals);
  }

  /* check if there is more parameter values than procedure arguments */
  if ( vals != Mg_emptyList ) {
    static const MgStatus error_invalid_number_of_args =
      { "procedure: too much arguments provided" };
    s = &error_invalid_number_of_args;
    goto error;
  }

  return MgList_evaluate_sequential(proc->code, output, interpreter, proc->environment);

  error:
  return s;
}

static MgStatus* represent(MgProcedure* proc,
                           FILE* fs) {
  fprintf(fs, "<procedure>");
  return Mg_ok;
}

static MgStatus* destroy(MgProcedure* proc) {
  return MgProcedure_destroy(proc);
}

static const MgObjectType type = {
  .evaluate = (MgObject_evaluate_func)evaluate,
  .evaluate_on = (MgObject_evaluate_on_func)evaluate_on,
  .represent = (MgObject_represent_func)represent,
  .destroy = (MgObject_destroy_func)destroy
};

MgStatus* MgProcedure_create(MgProcedure** procedure,
                             MgList* argument,
                             MgList* code,
                             MgEnv* parent_env) {
  MgStatus* s;

  MgProcedure* new_procedure = malloc(sizeof(MgProcedure));
  if (new_procedure == NULL) {
    s = Mg_error_malloc;
    goto error;
  }

  /* create procedure environment */
  MgEnv* env;
  s = MgEnv_create(&env, parent_env);
  if ( s != Mg_ok ) goto free_procedure_and_error;
  
  /* object fields */
  new_procedure->base.refcnt = 0;
  new_procedure->base.type = &type;
  
  /* procedure fields*/
  new_procedure->argument = argument;
  MgObject_add_reference((MgObject*)argument);
  
  new_procedure->code = code;
  MgObject_add_reference((MgObject*)code);
  
  new_procedure->environment = env;
  MgObject_add_reference((MgObject*)env);
  

  *procedure = new_procedure;
  return Mg_ok;

  free_procedure_and_error:
  free(new_procedure);
  error:
  return s;
}

MgStatus* MgProcedure_destroy(MgProcedure* procedure) {
  MgObject_drop_reference((MgObject*)procedure->argument);
  MgObject_drop_reference((MgObject*)procedure->code);
  MgObject_drop_reference((MgObject*)procedure->environment);
  free(procedure);
  return Mg_ok;
}

int Mg_is_a_procedure(const MgObject* obj) {
  return obj->type == &type ? 1 : 0;
}
