#include "MgPrimitives.h"

#include <stdlib.h>

struct MgPrimitives{ MgObject base;
	MgPrimitives_Global_Func func;
};

static MgStatus * evaluate(MgPrimitives * prim, MgObject ** output,
					MgInterpreter * interpreter,
					MgEnv * Env){
	*output = (MgObject *)prim;
	return Mg_ok;
}

static MgStatus * evaluate_on(MgPrimitives * prim,
					   MgPrimitives * target,
					   MgObject ** output,
					   MgInterpreter * interpreter,
					   MgEnv * env){
	return prim->func(target, output, interpreter, env); 
}

MgStatus * represent(MgPrimitives * prim, FILE * fs){
	fprintf(fs, "<build-in procedure @ %p>", prim);
	return Mg_ok;
};

MgStatus * destroy(MgPrimitives * prim){
	return MgPrimitives_destroy(prim);
}

static const MgObjectType type = {
  .evaluate = (MgObject_evaluate_func)evaluate,
  .evaluate_on = (MgObject_evaluate_on_func)evaluate_on,
  .represent = (MgObject_represent_func)represent,
  .destroy = (MgObject_destroy_func)destroy
};

MgStatus * MgPrimitives_create(MgPrimitives ** primitives, 
								MgPrimitives_Global_Func func){
	MgPrimitives * new_primitive = malloc(sizeof(MgPrimitives));

	if (new_primitive == NULL){
		return Mg_error_malloc;
	}

	new_primitive->base.refcnt = 0;
	new_primitive->base.type = &type;

	new_primitive->func = func;

	* primitives = new_primitive;
	return Mg_ok;
}

MgStatus * MgPrimitives_destroy(MgPrimitives * prim){
	free(prim);
	return Mg_ok;
}