#ifndef MGENVIRONMENT_H
#define MGENVIRONMENT_H

#include "MgList.h"
#include "MgStatus.h"
#include "MgObject.h"
#include "MgIdentifier.h"

typedef MgList MgEnv;

/**
 * create an environment on top of a parent environment
 *
 * @param  env output
 * @return Mg_ok if ok
 */
MgStatus* MgEnv_create(MgEnv** env,
                       MgEnv* parent_env);


/**
 * destroy an environment.
 *
 * @param
 * @return Mg_ok if ok
 */
MgStatus* MgEnv_destroy(MgEnv* env);


/**
 * append environment with a new identifier
 *
 * @param env
 * @param identifier
 * @return Mg_ok if ok
 */
MgStatus* MgEnv_add_identifier(MgEnv** env,
                               MgIdentifier* identifier,
                               MgObject* binded_object,
			       int scope_limited);


/**
 * append environment with a new string identifier
 *
 * @param env 
 * @param identifier 
 * @param binded_object 
 * @return Mg_ok if ok
 */
MgStatus* MgEnv_add_identifier_from_string(MgEnv** env,
                                           const char* identifier,
                                           MgObject* binded_object,
					   int scope_limited);

/**
 * find an identifier in an environment and return the associated bond
 * with the variable
 *
 * @param env
 * @param identifier
 * @param scope_limited limit scope to the current env
 * @param bond a pair (identifier, object)
 * @return Mg_ok if found, MgEnv_error_identifier_not_found if not
 */
MgStatus* MgEnv_find_bond_from_identifier(const MgEnv* env,
                                          const char* identifier,
                                          MgPair** bond,
					  int scope_limited);


/**
 * unspecify any bond that link to a procedure that depends on the env
 *
 * @param env 
 * @param recursive 
 * @return 
 */
MgStatus* MgEnv_clean_dependency_loops(MgEnv* env, int recursive);

extern const MgStatus* MgEnv_error_identifier_not_found;

#endif /* MGENVIRONMENT_H */
