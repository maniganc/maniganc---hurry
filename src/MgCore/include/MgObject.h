#ifndef MGOBJECT_H
#define MGOBJECT_H
/**
 *   \file MgObject.h
 *   \brief Maniganc object description
 *
 *  Describes an object structure à la python.
 *
 */

#include <string.h>
#include <stdio.h>

#include "MgStatus.h"


#define debug_object(obj, ...)                                          \
  do {                                                                  \
    if(DEBUG) {                                                         \
      fprintf(stderr, "\n");                                            \
      fprintf(stderr, "[debug %s:%d %s()]", __FILE__, __LINE__, __func__); \
      fprintf(stderr, __VA_ARGS__);                                     \
      fprintf(stderr, "\n");                                            \
      MgObject_represent((MgObject*)obj, stderr);                       \
      fprintf(stderr, "\n");                                            \
    }                                                                   \
  }                                                                     \
  while (0);


typedef size_t Mg_size_t;

typedef struct MgObject MgObject;

/* forward declaration to avoid dependencies circles */
typedef struct MgInterpreter MgInterpreter;
typedef struct MgList MgList;
typedef MgList MgEnv;

typedef MgStatus* (*MgObject_evaluate_func)(MgObject* self,
                                            MgObject** output,
                                            MgInterpreter* interpreter,
                                            MgEnv* env);

typedef MgStatus* (*MgObject_evaluate_on_func)(MgObject* self,
                                               MgObject* target,
                                               MgObject** output,
                                               MgInterpreter* interpreter,
                                               MgEnv* env);

typedef MgStatus* (*MgObject_represent_func)(MgObject* self,
                                             FILE* fs);

typedef MgStatus* (*MgObject_is_equal_to_func)(MgObject* self,
                                               MgObject* other,
                                               FILE* fs);

typedef MgStatus* (*MgObject_destroy_func)(MgObject* self);

typedef struct {
MgObject_evaluate_func evaluate;
MgObject_evaluate_on_func evaluate_on;
MgObject_represent_func represent;
MgObject_is_equal_to_func is_equal;
MgObject_destroy_func destroy;
}MgObjectType;

struct MgObject {
Mg_size_t refcnt;
const MgObjectType* type;
};

/**
 * print the representation of an object
 *
 * @param self
 * @param fs filestream to use
 * @return Mg_ok if no errors
 */
MgStatus* MgObject_represent(MgObject* self, FILE* fs);

/**
 * evaluate an object into another.
 * the result of this evaluation cannot be a reference.
 * this function evaluate reference objects until the evaluation
 * is not a reference.
 *
 * @param self
 * @param output
 * @return Mg_ok if no errors
 */
MgStatus* MgObject_evaluate(MgObject* self, MgObject** output,
                            MgInterpreter* interpreter,
                            MgEnv* env);

/**
 * apply an object on another object
 *
 * @param self
 * @param target
 * @param output
 * @return
 */
MgStatus* MgObject_evaluate_on(MgObject* self,
                               MgObject* target,
                               MgObject** output,
                               MgInterpreter* interpreter,
                               MgEnv* env);

/**
 * destroy the object without checking its reference counter.
 *
 * @param obj
 * @return Mg_ok if no errors
 */
MgStatus* MgObject_destroy(MgObject* obj);

/**
 * reference an object
 *
 * @param obj
 * @return
 */
MgStatus* MgObject_add_reference(MgObject* obj);

/**
 * dereference an object
 * decrement reference counter and destroy object if it reaches zero
 *
 * @param obj
 * @return
 */
MgStatus* MgObject_drop_reference(MgObject* obj);

#define MG_OBJECT(obj_ptr) ((MgObject*)(obj_ptr))
#define MG_OBJECTTYPE(obj_ptr) ((MgObjectType*)(obj_ptr))

#define MG_REFCNT(obj_ptr) (((MgObject*)(obj_ptr))->refcnt)
#define MG_TYPE(obj_ptr) (((MgObject*)(obj_ptr))->type)

MgStatus* MgObject_drop(MgObject* obj);
#define MG_OBJECT_ADD_REF(obj_ptr) (++((MgObject*)(obj_ptr))->refcnt)
#define MG_OBJECT_DROP_REF(obj_ptr) MgObject_drop_reference(MG_OBJECT(obj_ptr))

extern const MgStatus* Mg_error_object_not_applicable;

#endif /* MGOBJECT_H */
