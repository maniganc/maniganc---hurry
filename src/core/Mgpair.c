
#include "MgPair.h"


MgStatus* MgPair_create(MgPair** pair,
			MgObject* car,
			MgObject* cdr) {
  MgList* list;
  MgStatus* status = MgList_create(&list);
  if (status != Mg_ok) goto error;

  status = MgList_push_front(&list, car);
  if (status != Mg_ok) goto destroy_and_error;

  /* replace cdr in list */
  status = MgList_set_cdr(list, cdr);
  if (status != Mg_ok) goto destroy_and_error;

  *pair = list;
  
  return Mg_ok;
  
 destroy_and_error:
  MgList_destroy(list);
 error:
  return status;
}
