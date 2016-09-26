#ifndef SAVED_STREAM_H
#define SAVED_STREAM_H

#define TYPE char
#include "vector_template.h"

#include <string.h>

typedef int (*MgSavedStream_getchar_func)(void* getchar_payload);

typedef struct {
  /* function used to get next char from a user-defined stream */
  MgSavedStream_getchar_func getchar;

  void* getchar_payload;

  vector_char buffer;

  int buffer_char_flag;

  size_t current_char_idx;

  size_t mark_counter;

  int init;

}MgSavedStream;

typedef size_t MgSavedStream_mark;

void MgSavedStream_init(MgSavedStream* s,
                        MgSavedStream_getchar_func getchar,
                        void* getchar_payload);

void MgSavedStream_deinit(MgSavedStream* s);

void MgSavedStream_reset(MgSavedStream* s);

/**
 * get current char
 *
 * @param s
 * @return
 */
int MgSavedStream_get_current(MgSavedStream* s);

int MgSavedStream_get_next(MgSavedStream* s);

int MgSavedStream_get_previous(MgSavedStream* s);

MgSavedStream_mark MgSavedStream_mark_current(MgSavedStream*s);

void MgSavedStream_load_mark(MgSavedStream*s, MgSavedStream_mark mark);

void MgSavedStream_drop_mark(MgSavedStream*s);

#endif /* SAVEDSTREAM_H */
