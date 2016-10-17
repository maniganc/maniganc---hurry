
#include "buildin_procedures.h"

#include "MgList.h"

MG_BUILDIN_PROCEDURE(quote, "quote") {
  *output = MgList_get_car((MgList*)arg);
  return Mg_ok;
}
