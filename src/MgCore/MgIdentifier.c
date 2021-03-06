
#include "MgIdentifier.h"
#include "debug.h"
#include <stdlib.h>

#include "MgInterpreter.h"
#include "MgEnvironment.h"

struct MgIdentifier { MgObject base;
  char* name;
};

static MgStatus* evaluate(MgIdentifier* self, MgObject** output,
			  MgInterpreter* interpreter,
			  MgEnv* env){
  /* find associated object */
  MgPair* bond;
  MgStatus* s;
  s = MgEnv_find_bond_from_identifier(env, MgIdentifier_get_name(self), &bond, 0);
  if (s != Mg_ok) goto error;

  MgObject* associated_object = MgList_get_cdr(bond);
  
  *output = associated_object;
  return Mg_ok;

 error:
  return s;
}

static MgStatus* evaluate_on(MgIdentifier* self, MgIdentifier** output){
  return Mg_error_object_not_applicable;
}

static MgStatus* destroy(MgIdentifier* identifier) {
  return MgIdentifier_destroy(identifier);
}

static MgStatus* represent(MgIdentifier* identifier, FILE* fs) {
  fprintf(fs, "%s", identifier->name);
  return Mg_ok;
}

static const MgObjectType type = {
  .evaluate = (MgObject_evaluate_func)evaluate,
  .evaluate_on = (MgObject_evaluate_on_func)evaluate_on,
  .represent = (MgObject_represent_func)represent,
  .destroy = (MgObject_destroy_func)destroy
};
const MgObjectType* MgIdentifier_type = &type;


MgStatus* MgIdentifier_create_from_string(MgIdentifier** identifier,
					  const char* name) {
  MgIdentifier* new_identifier = malloc(sizeof(MgIdentifier));
  if (new_identifier == NULL) {
    return Mg_error_malloc;
  }

  new_identifier->base.refcnt = 0;
  new_identifier->base.type = MgIdentifier_type;
  new_identifier->name = strdup(name);

  *identifier = new_identifier;

  return Mg_ok;
}

MgStatus* MgIdentifier_destroy(MgIdentifier* identifier) {
  free(identifier->name);
  free(identifier);
  return Mg_ok;
}

const char* MgIdentifier_get_name(const MgIdentifier* identifier) {
  return identifier->name;
}

int Mg_is_an_identifier(MgObject* obj) {
  return obj->type == MgIdentifier_type ? 1 : 0;
}

MgStatus* MgIdentifier_parser_func(MgSavedStream* ss,
                                   const  MgObjectParser** object_parsers,
                                   MgIdentifier** output_identifier) {
  /* do not create an objet yet : due to bad symbol ending,
   * object would have to be freed if created */
  vector_char temp_name;
  vector_char_init(&temp_name);

  int begin = 1;

  do {
    int c = MgSavedStream_get_current(ss);
    if ( ('A' <= c && c <= 'Z') ||
         ('a' <= c && c <= 'z') ||
         c == '!' ||
         c == '$' ||
         c == '%' ||
         c == '&' ||
         c == '*' ||
         c == '+' ||
         c == '-' ||
         c == '.' ||
         c == '/' ||
         c == ':' ||
         c == '<' ||
         c == '=' ||
         c == '>' ||
         c == '?' ||
         c == '@' ||
         c == '^' ||
         c == '_' ||
         c == '~' ||
	 (('0' <= c && c <= '9') && !begin) ) {
      vector_char_push(&temp_name, c);

      begin = 0;
      
      c = MgSavedStream_get_next(ss);
      if (c < 0) {
        return MgParser_error_getchar;
      }
    }

    else {
      /* something else */
      break;
    }
  }
  while(1);

  if (vector_char_get_size(&temp_name) == 0) {
    return MgParser_error_syntax;
  }

  /* finish it to from a valid string */
  vector_char_push(&temp_name, '\0');

  /* already outside symbol */
  /* check ending */
  MgStatus* ending_status =  MgParser_check_symbol_ending(ss);

  if (ending_status == Mg_ok) {
    /* return creation status */
    MgStatus* s;
    s = MgIdentifier_create_from_string(output_identifier,
                                        vector_char_get_raw_ptr(&temp_name));
    vector_char_deinit(&temp_name);
    return s;
  }
  else {
    return MgParser_error_syntax;
  }
}

const MgObjectParser MgIdentifier_parser = {
  .parse = (MgObjectParser_func)MgIdentifier_parser_func
};
