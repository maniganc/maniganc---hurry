
#include "procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"

static const MgStatus error_no_object = {
	.message = "The multiplication operator requires an object"
};

MG_PROCEDURE(mult, "*") {
	return Mg_ok;
}