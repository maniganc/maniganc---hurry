
#include "procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"

static const MgStatus error_no_object = {
	.message = "The remainder operator requires an object"
};

MG_PROCEDURE(remainder, "remainder") {
	return Mg_ok;
}