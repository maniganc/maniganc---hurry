
#include "MgList.h"
#include "MgQuote.h"
#include <stdlib.h>
#include "debug.h"
static const MgStatus error_improper = {
  .message = "List is improper"
};
const MgStatus* MgList_error_improper = &error_improper;


struct MgList { MgObject base;
  MgObject* car;
  MgObject* cdr;
};

static MgStatus* evaluate(MgList* list, MgObject** output) {
  return Mg_error_not_implemented;
}


static MgStatus* evaluate_on(MgList* list,
                             MgObject* target,
                             MgObject** output) {
  return Mg_error_not_implemented;
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

    else {
      /* not the end, something else */
      /* must be an object, try to parse it */
      MgObject* obj;
      MgStatus* s = MgParser_parse_object(ss, object_parsers, &obj);

      if (s == Mg_ok) {
        /* object successully parsed, append it to the list*/
        MgList_push_back(&list_tail, obj);
        if (MgList_is_empty(list_head)) {
          /* at the beginning, list_head is empty.
           * the first push_back creates a valid pair which should be the new
           * head.
           * the everything is pretty normal : list_head stays there while
           * the tail falls in the darkness */
          list_head = list_tail;
        }
        /* skip sugar */
        MgParser_skip_sugar(ss, NULL);
      }

      else {
        /* parsing failed */
        MgList_destroy(list_head);
        return s;
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

MgObject* MgList_get_car(MgList* list) {
  return list->car;
}

MgObject* MgList_get_cdr(MgList* list) {
  return list->cdr;
}

MgStatus* MgList_push_front(MgList** list_head, MgObject* object) {
  MgPair* new_front_pair = malloc(sizeof(MgPair));
  if (new_front_pair == NULL) {
    return Mg_error_malloc;
  }

  *new_front_pair = emptylist;

  new_front_pair->car = object;
  MG_OBJECT_ADD_REF(object);

  new_front_pair->cdr = MG_OBJECT(*list_head);
  MG_OBJECT_ADD_REF(*list_head);

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

  (*list_tail)->cdr = (MgObject*)new_tail_pair;
  MG_OBJECT_ADD_REF(new_tail_pair);

  new_tail_pair->car = object;
  MG_OBJECT_ADD_REF(object);

  new_tail_pair->cdr = (MgObject*)Mg_emptyList;
  /* no need to increase empylist ref,
   * as we omitted to decrease it when list_tail->cdr was changed  */

  *list_tail = new_tail_pair;
  return Mg_ok;
}

MgStatus* MgList_destroy(MgList* list) {
  if (MgList_is_empty(list)) {
    return Mg_ok;
  }

  MG_OBJECT_DROP_REF(list->car);
  MG_OBJECT_DROP_REF(list->cdr);

  free(list);

  return Mg_ok;
}
