
#include "MgInterpreter.h"
#include <stdlib.h>


/* TODO: relocate that in __parsers__ list */
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

#include "MgBuildInProcedure.h"
#include "buildin_procedures.h"

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

  /* interpreter needs symbol env */
  MgObject_add_reference((MgObject*)new_interpreter->symbol_env);

  /* create parser list */
  MgList* parser_list;
  status = MgList_create(&parser_list);
  if (status != Mg_ok) goto destroy_env_and_more_and_error;
  /* add it to environment */
  MgEnv_add_identifier_from_string(&new_interpreter->symbol_env,
                                   "__parsers__", (MgObject*)parser_list, 0);

  MgIdentifier* id;
  MgIdentifier_create_from_string(&id, "debug");
  MgEnv_add_identifier_from_string(&new_interpreter->symbol_env,
                                   "__debug__", (MgObject*)id, 0);

  /* load buildin procedures */
  const Mg_buildin_procedure* pair_proc_name = &Mg_buildin_procedure_array[0];
  while (pair_proc_name->func != NULL) {
    MgBuildInProcedure* bproc;
    MgBuildInProcedure_create(&bproc, pair_proc_name->func);
    MgEnv_add_identifier_from_string(&new_interpreter->symbol_env,
                                     pair_proc_name->name,
                                     (MgObject*)bproc, 0);
    pair_proc_name++;
  }

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
  MgObject_drop_reference((MgObject*)interpreter->symbol_env);
  free(interpreter);
  return Mg_ok;
}

MgStatus* MgInterpreter_evaluate_sstream(MgInterpreter* interpreter,
                                         MgSavedStream* ss,
                                         int interactive_mode,
                                         int parse_only_mode) {

  if (interactive_mode) printf("$ ");

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
      /* use output object */
      MgObject_add_reference(output_object);

      if (interactive_mode) printf("==> ");

      if (parse_only_mode) {
        MgObject_represent(output_object, stdout);
	printf("\n");
      } /* parse-only mode */

      else {
        MgObject* evaluated_obj;
        s = MgObject_evaluate(output_object, &evaluated_obj,
                              interpreter,
                              MgInterpreter_get_symbol_environment(interpreter));

        if (s != Mg_ok) {
          /* failed to evaluate output object */
	  fprintf(stderr, "error at line %ld: %s\n",
              MgSavedStream_get_line_number(ss)-1,
              s->message);
	  if (!interactive_mode) {
	    /* exit */
	    MgObject_drop_reference(output_object);
	    return s;
	  }
        }

        else {
          /* use evaluated object */
          MgObject_add_reference(evaluated_obj);
          /* print it */
          if (interactive_mode) MgObject_represent(evaluated_obj, stdout);
          /* evaluated object is now useless, drop it */
          MgObject_drop_reference(evaluated_obj);
        }
      } /* evaluation mode */

      /* output object is now useless, drop it */
      MgObject_drop_reference(output_object);
    }

    if (interactive_mode) printf("\n$ ");
  }

  return Mg_ok;
}

MgStatus* MgInterpreter_evaluate_stream(MgInterpreter* interpreter,
                                        FILE* fs,
                                        int interactive_mode,
                                        int parse_only_mode) {
  MgSavedStream ss;
  MgSavedStream_init(&ss, (MgSavedStream_getchar_func)fgetc, fs);
  MgStatus* status = MgInterpreter_evaluate_sstream(interpreter,
                                                    &ss,
                                                    interactive_mode,
                                                    parse_only_mode);
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

MgList* MgInterpreter_get_emptylist(const MgInterpreter* interpreter) {
  return interpreter->emptylist;
}
