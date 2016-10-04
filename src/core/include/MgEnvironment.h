#ifndef MGENVIRONMENT_H
#define MGENVIRONMENT_H

#include "MgList.h"
#include "MgStatus.h"
#include "MgObject.h"
#include "MgIdentifier.h"

typedef MgList MgEnv;

/**
 * create an environment.
 *
 * @param  env output
 * @return Mg_ok if ok
 */
MgStatus* MgEnv_create(MgEnv** env);


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
			       const MgIdentifier* identifier,
			       const MgObject* binded_object);

/**
 * find an identifier in an environment and return the associated object
 *
 * @param env 
 * @param identifier 
 * @return Mg_ok if ok
 */
MgStatus* MgEnv_get_object_from_identifier(const MgEnv* env,
					   const MgIdentifier* identifier,
					   MgObject** output_object);


extern const MgStatus* MgEnv_error_identifier_not_found;

#endif /* MGENVIRONMENT_H */
