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
 * @param  
 * @return Mg_ok if ok
 */
MgStatus* MgEnv_create(void);


/**
 * destroy an environment.
 *
 * @param  
 * @return Mg_ok if ok
 */
MgStatus* MgEnv_destroy(void);


/**
 * append environment with a new identifier
 *
 * @param env 
 * @param identifier 
 * @return Mg_ok if ok
 */
MgStatus* MgEnv_add_identifier(MgEnv* env, MgIdentifier* identifier);

/**
 * find an identifier in an environment and return the associated object
 *
 * @param env 
 * @param identifier 
 * @return Mg_ok if ok
 */
MgStatus* MgEnv_get_object_from_identifier(MgEnv* env,
					   MgIdentifier* identifier,
					   MgObject** output_object);

#endif /* MGENVIRONMENT_H */
