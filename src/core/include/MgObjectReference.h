#ifndef MGOBJECTREFERENCE_H
#define MGOBJECTREFERENCE_H
/**
 *   \file MgObjectReference.c
 *   \brief an object that holds the reference of another object
 *
 * used when the evaluation have to be returned but is not evaluated
 * by the current function, ex: tail call optimisation.
 *
 */

#include "MgObject.h"

typedef struct MgObjectReference MgObjectReference;

/**
 * create a reference object
 *
 * @param ref 
 * @return 
 */
MgStatus* MgObjectReference_create(MgObjectReference** ref);

/**
 * destroy a reference object
 *
 * @param ref 
 * @return 
 */
MgStatus* MgObjectReference_destroy(MgObjectReference* ref);

/**
 * 0 if the object is not a reference.
 *
 * @param obj 
 * @return 
 */
int Mg_is_a_reference(const MgObject* obj);

/**
 * update the reference object of an interpreter.
 * when evaluated, the reference object will evaluate the object
 * in this environment.
 * the result of this delayed evaluation can be another reference.
 *
 * this is an equivalent of MgObject.h:MgObject_evaluate that is
 * used mainly for tail call optimisation.
 *
 * @param object
 * @param object_ref
 * @param interpreter
 * @param env
 * @return Mg_ok if no errors
 */
MgStatus* MgObjectReference_return_ref(MgObject* object,
                                       MgObjectReference** object_ref,
                                       MgInterpreter* interpreter,
                                       MgEnv* env);
#endif /* MGOBJECTREFERENCE_H */
