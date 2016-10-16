
#include "MgInterpreter.h"
#include <stdlib.h>

#include "MgList.h"
#include "MgBool.h"
#include "MgInteger.h"
#include "MgIdentifier.h"
#include "MgChar.h"
#include "MgString.h"
#include "MgQuote.h"
const MgObjectParser* object_parsers[] = {
  &MgString_parser,
  &MgList_parser,
  &MgBool_parser,
  &MgInteger_parser,
  &MgIdentifier_parser,
  &MgChar_parser,
  &MgQuote_parser,
  NULL
};

struct MgInterpreter {
  MgEnv* symbol_env;
  MgList* emptylist;
};

MgStatus* MgInterpreter_create(MgInterpreter** interpreter) {
  MgStatus* status;

  MgInterpreter* new_interpreter = malloc(sizeof(MgInterpreter));
  if (new_interpreter == NULL) {
    return Mg_error_malloc;
  }

  /* TODO: create new emptylist for each interpreter */
  status = MgList_create(&new_interpreter->emptylist);
  if (status != Mg_ok) goto error;

  /* TODO: what about true and false ? */

  /* create symbol environment */
  status = MgEnv_create(&new_interpreter->symbol_env,
                        new_interpreter->emptylist); /* parent is the emptylist */
  if (status != Mg_ok) goto destroy_list_and_error;

  /* create parser list */
  MgList* parser_list;
  status = MgList_create(&parser_list);
  if (status != Mg_ok) goto destroy_env_and_more_and_error;
  /* add it to environment */
  MgEnv_add_identifier_from_string(&new_interpreter->symbol_env,
                                   "__parsers__", (MgObject*)parser_list);

  *interpreter = new_interpreter;
  return Mg_ok;

 destroy_env_and_more_and_error:
  MgEnv_destroy(new_interpreter->symbol_env);
 destroy_list_and_error:
  MgList_destroy(new_interpreter->emptylist);
 error:
  return status;
}

MgStatus* MgInterpreter_destroy(MgInterpreter* interpreter) {
  MgList_destroy(interpreter->emptylist);
  MgEnv_destroy(interpreter->symbol_env);
  return Mg_ok;
}

MgStatus* MgInterpreter_evaluate_sstream(MgInterpreter* interpreter,
                                        MgSavedStream* ss,
                                        int interactive_mode) {
  printf("$ ");

  while(1) {
    if (MgSavedStream_get_current(ss) == EOF) {
      break;
    }

    MgParser_skip_sugar(ss, NULL);

    if (MgSavedStream_get_current(ss) == EOF) {
      break;
    }
    MgObject* output_object;
    MgStatus* s =  MgParser_parse_object(ss, object_parsers, &output_object);
    if (s != Mg_ok) {
      fprintf(stderr, "error at line %ld: %s\n",
              MgSavedStream_get_line_number(ss),
              s->message);

      /* flush stdin */
      int ch; while ((ch = getchar()) != '\n' && ch != EOF);
      MgSavedStream_reset(ss);
    }

    else {
      printf("==> ");
#ifdef DEBUG
      MgObject_represent(output_object, stdout);
#endif // DEBUG
      MgObject* evaluated_obj;
      s = MgObject_evaluate(output_object, &evaluated_obj);
      if (s != Mg_ok) {
        /* failed to represent output object */
        fprintf(stderr, "error: %s\n", s->message);

      }
      else {
        MgObject_represent(output_object, stdout);
        /* for now, evaluated object is useless, delete it */
        if (evaluated_obj != output_object) {
          /* evaluated object can be the same as output object
           * avoid double-free */
          MgObject_destroy(evaluated_obj);
        }
      }
      MgObject_destroy(output_object); /* for now, object is useless */
    }


    printf("\n$ ");
  }

  return Mg_ok;
}

MgStatus* MgInterpreter_evaluate_stream(MgInterpreter* interpreter,
                                       FILE* fs,
                                       int interactive_mode) {
  MgSavedStream ss;
  MgSavedStream_init(&ss, (MgSavedStream_getchar_func)getchar, NULL);
  MgStatus* status = MgInterpreter_evaluate_sstream(interpreter,
						    &ss,
						    interactive_mode);
  MgSavedStream_deinit(&ss);
  return status;
}

MgStatus* MgInterpreter_evaluate_file(MgInterpreter* interpreter,
                                     const char* filepath) {

}

MgStatus* MgInterpreter_evaluate_string(MgInterpreter* interpreter,
                                       const char* str) {

}

MgEnv* MgInterpreter_get_symbol_environment(const MgInterpreter* interpreter) {
  return interpreter->symbol_env;
}
