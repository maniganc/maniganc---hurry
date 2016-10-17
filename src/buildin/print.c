
#include "buildin_procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"
#include "MgInteger.h"

MG_BUILDIN_PROCEDURE(pp, "pp") {
  if ((MgList*)arg == MgInterpreter_get_emptylist(interpreter)) {
    return Mg_ok;
  }
  
  MgObject_represent(MgList_get_car((MgList*)arg), stdout);
  fprintf(stdout, "\n");
  return Mg_ok;
}
