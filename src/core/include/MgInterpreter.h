
#include "MgStatus.h"
#include "MgEnvironment.h"

typedef struct MgInterpreter MgInterpreter;

/**
 * create a manignac interpreter
 *
 * @param interpreter 
 * @return 
 */
MgStatus* MgInterpreter_create(MgInterpreter** interpreter);


/**
 * destroy a manignac interpreter
 *
 * @param interpreter 
 * @return 
 */
MgStatus* MgInterpreter_destroy(MgInterpreter* interpreter);


/**
 * get interpreter environment
 *
 * @param interpreter 
 * @param env 
 * @return 
 */
MgStatus* MgInterpreter_get_environment(const MgInterpreter* interpreter,
					MgEnv** env);


/* MgStatus* MgStdEnv_define(MgList* args, MgInterpreter* interpreter) { */
  
/* } */


