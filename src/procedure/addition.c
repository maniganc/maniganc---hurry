
#include "procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"

static const MgStatus error_no_object = {
	.message = "The addition operator requires an object"
};

/*
* 
*  Addition, as any other arithmetic operation, requires at least 
*  2 operands.
*  
*  At first, we will define the car and cdar both as non-symbolic.
*  We need to create an algorithm that reaches the last cdar that is 
*  a non-list, then apply the operation + between the last car and 
*  the last cdr. The result will be sent back one level up to apply 
*  the same operation with the upper level car, and so on.
*
*/

MG_PROCEDURE(addition, "+") {
    MgStatus* status;

    /* Get the car */
    MgObject* car = MgList_get_car((MgList*) arg);

    /* Cdar is also a non identifier (symbol), preassumed to be a list */
    MgList* obj_list = (MgList*)MgList_get_cdr((MgList*)arg);

    /* If we reached the end of the list, or no object left, "throw" 
     * an error
     */

    if(obj_list == MgInterpreter_get_emptylist(interpreter)) {
    	return &error_no_object;
    }

    *output = car; /* Avoids potential segmentation fault 
                      (for the moment) */ 
    return Mg_ok;
}