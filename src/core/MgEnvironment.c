
#include "MgEnvironment.h"

MgStatus* MgEnv_create(MgEnv** env) {
  return MgList_create(env);
}

MgStatus* MgEnv_destroy(MgEnv* env) {
  return MgList_destroy(env);
}

MgStatus* MgEnv_add_identifier(MgEnv** env,
                               const MgIdentifier* identifier,
                               const MgObject* binded_object) {
  MgStatus* s;

  /* create a bond between the identifier and the binded object */
  MgList* bond;
  s = MgList_create(&bond);
  if (s != Mg_ok) goto error;
  
  s = MgList_push_front(&bond, (const MgObject*)binded_object);
  if (s != Mg_ok) goto error_destroy;

  s = MgList_push_front(&bond, (const MgObject*)identifier);
  if (s != Mg_ok) goto error_destroy;

  /* append env with a new bond */
  s = MgList_push_front(env, (const MgObject*)bond);
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

  const char* find_id_name = MgIdentifier_get_name(identifier);

  do {
    MgList* bond = (MgList*)MgList_get_car(env);
    const char* id_name = MgIdentifier_get_name(id);

    if (strcmp(id_name, find_id_name) == 0) {
      /* *output_object = mgi */
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
extern const MgStatus* MgEnv_error_identifier_not_found =
  &error_identifier_not_found;
