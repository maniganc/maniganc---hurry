
#include "procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"

static const MgStatus error_no_object = {
	.message = "The substraction operator requires an object"
};

MG_PROCEDURE(substraction, "-") {
	return Mg_ok;
}