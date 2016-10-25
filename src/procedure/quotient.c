
#include "procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"

static const MgStatus error_no_object = {
	.message = "The quotient operator requires an object"
};

MG_PROCEDURE(quotient, "quotient") {
	return Mg_ok;
}