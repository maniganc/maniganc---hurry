
#include "MgEnvironment.h"
#include "MgPair.h"
#include "MgProcedure.h"
#include "MgUnspecified.h"

MgStatus* MgEnv_create(MgEnv** env,
                       MgEnv* parent_env) {
  MgStatus* status;

  status = MgList_create(env);
  if (status != Mg_ok) goto error;

  status = MgList_push_front(env, (MgObject*)parent_env);
  if (status != Mg_ok) goto destroy_and_error;

  return Mg_ok;

 destroy_and_error:
  MgList_destroy(*env);
 error:
  return status;
}

MgStatus* MgEnv_destroy(MgEnv* env) {
  return MgList_destroy(env);
}

MgStatus* MgEnv_add_identifier(MgEnv** env,
                               MgIdentifier* identifier,
                               MgObject* binded_object,
                               int scope_limited) {
  MgStatus* status;

  /* search if identifier already exists */
  MgPair* bond;
  status = MgEnv_find_bond_from_identifier(*env,
                                           MgIdentifier_get_name(identifier),
                                           &bond,
                                           scope_limited);
  if (status == Mg_ok) {
    /* found, replace binded object */
    MgList_set_cdr(bond, binded_object);
    return Mg_ok;
  }

  /* create a bond between the identifier and the binded object */
  status = MgPair_create(&bond, (MgObject*)identifier, binded_object);
  if (status != Mg_ok) goto error;

  /* append env with a new bond */

  /* first car is the parent env, skip it */
  MgList* env_bond_list = (MgList*)MgList_get_cdr(*env);
  status = MgList_push_front(&env_bond_list, (MgObject*)bond);
  if (status != Mg_ok) goto destroy_and_error;

  /* update bond list in env */
  MgList_set_cdr(*env, (MgObject*)env_bond_list);

  return Mg_ok;

  /* errors */
 destroy_and_error:
  MgList_destroy(bond);
 error:
  return status;
}

MgStatus* MgEnv_add_identifier_from_string(MgEnv** env,
                                           const char* identifier,
                                           MgObject* binded_object,
                                           int scope_limited) {
  MgIdentifier* id;
  MgStatus* status;
  status = MgIdentifier_create_from_string(&id, identifier);
  if (status != Mg_ok) goto error;

  status = MgEnv_add_identifier(env, id, binded_object, scope_limited);
  if (status != Mg_ok) goto destroy_id_and_error;

  return Mg_ok;

 destroy_id_and_error:
  MgIdentifier_destroy(id);
 error:
  return status;
}


const MgStatus error_invalid_environment = {
  .message = "invalid environment"
};


MgStatus* MgEnv_find_bond_from_identifier(const MgEnv* env,
                                          const char* identifier,
                                          MgPair** output_bond,
                                          int scope_limited) {
  if (env == Mg_emptyList) {
    return MgEnv_error_identifier_not_found;
  }

  const char* target_id_name = identifier;

  MgList* bond_list = (MgList*)MgList_get_cdr(env);

  while(bond_list != Mg_emptyList){
    MgList* bond = (MgList*)MgList_get_car((MgList*)bond_list);
    if (!Mg_is_a_list((MgObject*)bond)) return &error_invalid_environment;

    MgIdentifier* id = (MgIdentifier*)MgList_get_car(bond);
    if (!Mg_is_an_identifier((MgObject*)id)) return &error_invalid_environment;

    const char* id_name = MgIdentifier_get_name(id);

    if (strcmp(id_name, target_id_name) == 0) {
      *output_bond = bond;
      return Mg_ok;
    }

    /* next bond */
    bond_list = (MgList*)MgList_get_cdr(bond_list);
  }

  /* nothing in the current env */
  if (scope_limited) {
    return MgEnv_error_identifier_not_found;
  }

  /* could not find the identifier in the current environment
   * search again in the parent env */
  MgEnv* parent_env = (MgEnv*)MgList_get_car(env);
  return MgEnv_find_bond_from_identifier(parent_env,
                                         identifier,
                                         output_bond,
                                         scope_limited);
}

MgStatus* MgEnv_clean_dependency_loops(MgEnv* env, int recursive) {
  if (env == Mg_emptyList) {
    return Mg_ok;
  }

  MgList* bond_list = (MgList*)MgList_get_cdr(env);
  while(bond_list != Mg_emptyList){
    MgList* bond = (MgList*)MgList_get_car((MgList*)bond_list);
    if (!Mg_is_a_list((MgObject*)bond)) return &error_invalid_environment;

    MgObject* binded_obj = MgList_get_cdr(bond);

    if ( Mg_is_a_procedure(binded_obj) ) {
      MgProcedure* proc = (MgProcedure*)binded_obj;
      MgEnv* proc_env = MgProcedure_get_environment(proc);
      if ( proc_env != Mg_emptyList) {
        MgEnv* proc_env_parent = (MgEnv*)MgList_get_car(proc_env);
	if ( proc_env_parent == env ) {
	  /* procedure needs env, and the bond in the env need the procedure */
	  if ( recursive ) {
	    MgStatus* s = MgEnv_clean_dependency_loops(proc_env,
						       recursive);
	    if ( s != Mg_ok ) return s;
	  }
	  /* mark the bond as unspecified, procedure will be dropped */
	  MgList_set_cdr(bond, (MgObject*)Mg_unspecified);
	}
      }
    }
    /* next bond */
    bond_list = (MgList*)MgList_get_cdr(bond_list);
  }

  return Mg_ok;
}


const MgStatus error_identifier_not_found = {
  .message = "cannot find identifier"
};
const MgStatus* MgEnv_error_identifier_not_found = &error_identifier_not_found;
