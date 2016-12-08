
#include "buildin_procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"
#include "MgBool.h"

MG_BUILDIN_PROCEDURE(quote, "quote") {

  static const MgStatus error_params = {
    .message = "quote require one argument"
  };

  if ((MgList*)arg == MgInterpreter_get_emptylist(interpreter)) {
    return &error_params;
  }

  if ((MgList*)MgList_get_cdr((MgList*)arg)
      != MgInterpreter_get_emptylist(interpreter)) {
    return &error_params;
  }

  MgObject* obj = MgList_get_car((MgList*)arg);

  *output = obj;
  return Mg_ok;
}
