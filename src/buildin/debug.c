
#include "buildin_procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgEnvironment.h"
#include "MgInteger.h"

MG_BUILDIN_PROCEDURE(_refcnt, "_refcnt") {
  MgObject* car = MgList_get_car((MgList*)arg);
  MgInteger* i;
  MgInteger_create_from_int(&i, car->refcnt);

  *output = i;
  return Mg_ok;
}
