
#include "MgEnvironment.h"

#include "MgPair.h"

MgStatus* MgEnv_create(MgEnv** env) {
  return MgList_create(env);
}

MgStatus* MgEnv_destroy(MgEnv* env) {
  return MgList_destroy(env);
}

MgStatus* MgEnv_add_identifier(MgEnv** env,
                               MgIdentifier* identifier,
                               MgObject* binded_object) {
  MgStatus* s;

  /* create a bond between the identifier and the binded object */
  MgPair* bond;
  s = MgPair_create(&bond, (MgObject*)identifier, binded_object);
  if (s != Mg_ok) goto error;
  
  /* append env with a new bond */
  s = MgList_push_front(env, (MgObject*)bond);
  if (s != Mg_ok) goto error_destroy;

  return Mg_ok;

  /* errors */
 error_destroy:
    MgList_destroy(bond);    
 error:
    return s;
}

MgStatus* MgEnv_get_object_from_identifier(const MgEnv* env,
                                           const MgIdentifier* identifier,
                                           MgObject** output_object) {
  if (env == Mg_emptyList) {
    return MgEnv_error_identifier_not_found;
  }

  const char* target_id_name = MgIdentifier_get_name(identifier);
  
  do {
    MgList* bond = (MgList*)MgList_get_car((MgList*)env);
    MgIdentifier* id = (MgIdentifier*)MgList_get_car(bond);
    const char* id_name = MgIdentifier_get_name(id);

    if (strcmp(id_name, target_id_name) == 0) {
      *output_object = MgList_get_cdr(bond);
        return Mg_ok;
    }
    
    env = (MgEnv*)MgList_get_cdr(env);
  }
  while(env != Mg_emptyList);

  return MgEnv_error_identifier_not_found;
}

const MgStatus error_identifier_not_found = {
  .message = "cannot find identifier"
};
const MgStatus* MgEnv_error_identifier_not_found = &error_identifier_not_found;
