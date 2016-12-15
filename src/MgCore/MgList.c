// Test
#include "MgList.h"
#include "MgQuote.h"
#include <stdlib.h>
#include "debug.h"

#include "MgInterpreter.h"
#include "MgUnspecified.h"

static const MgStatus error_improper = {
  .message = "List is improper"
};
const MgStatus* MgList_error_improper = &error_improper;


struct MgList { MgObject base;
  MgObject* car;
  MgObject* cdr;
};

MgStatus* MgList_evaluate_sequential(MgList* list, MgObject** output,
                                     MgInterpreter* interpreter,
                                     MgEnv* env) {
  /* check if there is no list */
  if (list == Mg_emptyList) {
    *output = (MgObject*)Mg_unspecified;
    return Mg_ok;
  }

  /* iterate over args */
  MgObject* obj;
  do {
    /* get next object */
    obj = MgList_get_car(list);

    /* if it is the last object, we don't need to kwow its evaluation */
    if (MgList_get_cdr(list) == (MgObject*)Mg_emptyList) {
      return MgObjectReference_return_ref(obj,
                                          (MgObjectReference**)output,
                                          interpreter, env);
    }

    /* evaluate it */
    MgObject* obj_eval;
    MgStatus* s = MgObject_evaluate(obj, &obj_eval, interpreter, env);

    /* check errors */
    if (s != Mg_ok) {
      return s;
    }

    /* we used the object
     * ensure that the object get cleaned if it have to */
    MgObject_add_reference(obj_eval);
    MgObject_drop_reference(obj_eval);

    /* next list */
    list = (MgList*)MgList_get_cdr((MgList*)list);
  } while(1);

  return Mg_ok;
}

static MgStatus* evaluate(MgList* list, MgObject** output,
                          MgInterpreter* interpreter,
                          MgEnv* env) {
  MgStatus* status;
  if (list == MgInterpreter_get_emptylist(interpreter)) {
    *output = (MgObject*)list;
    return Mg_ok;
  }
  /* car must be an applicable object */
  MgObject* car = MgList_get_car(list);

  /* evaluate car */
  MgObject* car_evaluated;
  status = MgObject_evaluate(car, &car_evaluated,
                             interpreter,
                             env);
  if (status != Mg_ok) goto error;

  /* use car evaluation for a while */
  MgObject_add_reference(car_evaluated);

  /* evaluate car on cdr (args) */
  MgObject* application_output;
  status = MgObject_evaluate_on(car_evaluated,
                                MgList_get_cdr(list),
                                &application_output,
                                interpreter,
                                env);
  if (status != Mg_ok) goto drop_evaluate_and_error;

  /* car evaluation is useless now */
  MgObject_drop_reference(car_evaluated);

  *output = application_output;
  return Mg_ok;


 drop_evaluate_and_error:
  MgObject_drop_reference(car_evaluated);
 error:
  return status;
}


static MgStatus* evaluate_on(MgList* list,
                             MgObject* target,
                             MgObject** output,
                             MgInterpreter* interpreter,
                             MgEnv* env) {
  return Mg_error_object_not_applicable;
}

static MgStatus* represent(MgList* list,
                           FILE* fs) {
  if (MgList_is_empty(list)) {
    fprintf(fs, "()");
    return Mg_ok;
  }

  fprintf(fs, "(");

  MgList* list_iter = list;
  int end = 0;
  do {
    /* print car */
    MgObject* car = MgList_get_car(list_iter);
    MgStatus* status = MgObject_represent(car, fs);
    if (status != Mg_ok) {
      return Mg_error;
    }

    /* print cdr */
    MgObject* cdr = MgList_get_cdr(list_iter);
    if (Mg_is_a_list(cdr)) {
      if (MgList_is_empty((MgList*)cdr)) {
        /* end of a proper list */
        end = 1;
      }
      else {
        /* the list continues */
        list_iter = (MgList*)cdr;
        fprintf(fs, " ");
      }
    }
    else {
      /* another object, list is improper. print it and end representation*/
      fprintf(fs, " . ");
      MgObject_represent(cdr, fs);
      end = 1;
    }
  }
  while(!end);

  fprintf(fs, ")");
  return Mg_ok;
}


static MgStatus* destroy(MgList* list) {
  return MgList_destroy(list);
}

static const MgObjectType MgList_type = {
  .evaluate = (MgObject_evaluate_func)evaluate,
  .evaluate_on = (MgObject_evaluate_on_func)evaluate_on,
  .represent = (MgObject_represent_func)represent,
  .destroy = (MgObject_destroy_func)destroy
};

MgList emptylist = {
  .base = {
    .refcnt = 0,
    .type = &MgList_type
  },
  .car = NULL,
  .cdr = NULL
};
MgList* Mg_emptyList = &emptylist;


MgStatus* MgList_parser_func(MgSavedStream* ss,
                             const MgObjectParser** object_parsers,
                             MgList** output_list) {
  MgStatus* status;
  /* a list must begin with an open bracket */
  int c = MgSavedStream_get_current(ss);
  if (c < 0) {
    /* err */
    return MgParser_error_getchar;
  }
  else if (c != '(') {
    /* not a list */
    return MgParser_error_syntax;
  }

  /* should be a list */
  MgList* list_head;
  MgList_create(&list_head);
  MgList* list_tail = list_head;

  /* 0 : continue to parse objects
   * 1 : one last object to parse
   * 2 : no remaining object to parse */
  int last_object_to_parse = 0;

  /* go inside the bracket */
  MgSavedStream_get_next(ss);

  /* remove sugar (whitespace chars, comments) */
  MgParser_skip_sugar(ss, NULL);

  /* iterate over elements */
  int stop = 0;
  do {
    /* check list end */
    c = MgSavedStream_get_current(ss);
    if (c < 0) {
      /* err */
      MgList_destroy(list_head);
      return MgParser_error_getchar;
    }

    else if (c == ')') {
      /* end of list */
      stop = 1;
      /* go out of the brackets */
      MgSavedStream_get_next(ss);
    }

    else if (c == '.') {
      /* tail of an improper list */

      if (last_object_to_parse != 0) {
        /* one more points/objects */
        /* err */
        MgList_destroy(list_head);
        return MgParser_error_syntax;
      }

      /* get past the point */
      c = MgSavedStream_get_next(ss);
      if (c < 0) {
        /* err */
        MgList_destroy(list_head);
        return MgParser_error_getchar;
      }

      /* skip sugar */
      MgParser_skip_sugar(ss, NULL);
      c = MgSavedStream_get_current(ss);
      if (c < 0) {
        /* err */
        MgList_destroy(list_head);
        return MgParser_error_getchar;
      }

      /* next object is the last object to parse and must be the tail */
      last_object_to_parse = 1;
    }

    else {
      /* not the end, something else */
      /* must be an object, try to parse it */
      MgObject* obj;
      status = MgParser_parse_object(ss, object_parsers, &obj);

      if (status == Mg_ok) {
        if (last_object_to_parse == 0) {
          /* object successully parsed, append it to the list*/
          MgList_push_back(&list_tail, obj);
          if (MgList_is_empty(list_head)) {
            /* at the beginning, list_head is empty.
             * the first push_back creates a valid pair which should be the new
             * head.
             * then everything is pretty normal : list_head stays there while
             * the tail falls in the darkness */
            list_head = list_tail;
          }
          /* skip sugar */
          MgParser_skip_sugar(ss, NULL);

        }

        else if (last_object_to_parse == 1) {
          /* last object to parse as tail */
          MgList_set_cdr(list_tail, obj);

          /* skip sugar */
          MgParser_skip_sugar(ss, NULL);

          /* no more object to parse */
          last_object_to_parse = 2;
        }

        else {
          /* err */
          MgList_destroy(list_head);
          return MgParser_error_syntax;
        }

      }

      else {
        /* parsing failed */
        MgList_destroy(list_head);
        return status;
      }
    }
  }
  while (!stop);

  *output_list = list_head;
  return Mg_ok;
}

const MgObjectParser MgList_parser = {
  .parse = (MgObjectParser_func)MgList_parser_func
};

MgStatus* MgList_create(MgList** list) {
  *list = Mg_emptyList;
  return Mg_ok;
}

int Mg_is_a_list(MgObject* obj) {
  if (obj->type == &MgList_type) {
    return 1;
  }
  return 0;
}

int MgList_is_empty(MgList* list) {
  return list == &emptylist ? 1 : 0;
}

int MgList_is_proper(MgList* list) {
  /* the empty list is a proper list */
  if (MgList_is_empty(list)) {
    return 1;
  }
  MgObject* cdr = MgList_get_cdr(list);
  if (Mg_is_a_list(cdr)) {
    /* if the cdr is a list, recursively check if it is proper */
    return MgList_is_proper((MgList*)cdr);
  }
  /* if cdr is not a list, the list is an improper list */
  return 0;
}

MgObject* MgList_get_car(const MgList* list) {
  return list->car;
}

MgObject* MgList_get_cdr(const MgList* list) {
  return list->cdr;
}

MgStatus* MgList_set_car(MgList* list, MgObject* obj) {
  if (MgList_is_empty(list)) {
    return Mg_error;
  }

  /* drop current cdr */
  MG_OBJECT_DROP_REF(list->car);

  /* replace it */
  list->car = obj;
  MG_OBJECT_ADD_REF(obj);

  return Mg_ok;
}


MgStatus* MgList_set_cdr(MgList* list, MgObject* obj) {
  if (MgList_is_empty(list)) {
    return Mg_error;
  }

  /* drop current cdr */
  MG_OBJECT_DROP_REF(list->cdr);

  /* replace it */
  list->cdr = obj;
  MG_OBJECT_ADD_REF(obj);

  return Mg_ok;
}

MgStatus* MgList_push_front(MgList** list_head,
                            MgObject* object) {
  MgPair* new_front_pair = malloc(sizeof(MgPair));
  if (new_front_pair == NULL) {
    return Mg_error_malloc;
  }

  *new_front_pair = emptylist;
  new_front_pair->base.refcnt = 0;

  new_front_pair->car = object;
  MgObject_add_reference((MgObject*)object);

  new_front_pair->cdr = MG_OBJECT(*list_head);
  MgObject_add_reference((MgObject*)*list_head);

  *list_head = (MgList*)new_front_pair;
  return Mg_ok;
}

MgStatus* MgList_push_back(MgList** list_tail, MgObject* object) {
  if (*list_tail == Mg_emptyList) {
    return MgList_push_front(list_tail, object);
  }

  if ((*list_tail)->cdr != (MgObject*)Mg_emptyList) {
    return MgList_error_improper;
  }

  MgPair* new_tail_pair = malloc(sizeof(MgPair));
  if (new_tail_pair == NULL) {
    return Mg_error_malloc;
  }
  *new_tail_pair = emptylist;
  new_tail_pair->base.refcnt = 0;

  /* for debugging */
  new_tail_pair->car = (MgObject*)Mg_emptyList;
  new_tail_pair->cdr = (MgObject*)Mg_emptyList;

  (*list_tail)->cdr = (MgObject*)new_tail_pair;
  MgObject_add_reference((MgObject*)new_tail_pair);

  new_tail_pair->car = object;
  MgObject_add_reference((MgObject*)object);

  new_tail_pair->cdr = (MgObject*)Mg_emptyList;
  /* no need to increase empylist ref,
   * as we omitted to decrease it when list_tail->cdr was changed  */

  *list_tail = new_tail_pair;
  return Mg_ok;
}

MgStatus* MgList_destroy(MgList* list) {
  /* do not destroy the empty list */
  if (MgList_is_empty(list)) {
    return Mg_ok;
  }

  MgObject_drop_reference((MgObject*)list->car);
  MgObject_drop_reference((MgObject*)list->cdr);

  free(list);

  return Mg_ok;
}
