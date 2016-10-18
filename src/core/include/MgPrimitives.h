#ifndef MGBUILDINPRIMITIVES_H_
#define MGBUILDINPRIMITIVES_H_

#include "MgObject.h"

typedef struct MgPrimitives MgPrimitives;

typedef MgStatus * (* MgPrimitives_Global_Func ) (MgObject * arg,
								MgObject** output,
								MgInterpreter* interpreter,
								MgEnv* env);

MgStatus * MgPrimitives_create(MgPrimitives ** primitives,
							 MgPrimitives_Global_Func func);

MgStatus * MgPrimitives_destroy(MgPrimitives * primitives);

#endif /* MGBUILDINPRIMITIVES_H_ */