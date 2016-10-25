
#include "procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"

static const MgStatus error_no_object = {
	.message = "The addition operator requires an object"
};

MG_PROCEDURE(addition, "+") {
	return Mg_ok;
}