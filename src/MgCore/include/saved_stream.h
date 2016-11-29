#ifndef SAVED_STREAM_H
#define SAVED_STREAM_H

#define TYPE char
#include "vector_template.h"

#include <string.h>

typedef struct {
  /* function used to get next char in a user-defined stream */
  int (*func_get_next_char)(void* func_payload, int* c);
  void* func_payload;

  /* previous characters buffer */
  vector_char previous_char_buffer;

  size_t last_stored_char_size;
  size_t next_char_to_store_idx;
  
  /* count number of requested checkpoints */
  size_t checkpoint_cnt;

  char last_char;

  int save_char;
}saved_stream_t;


void saved_stream_init(saved_stream_t* s,
                       int (*func_get_next_char)(void* payload, int* c),
                       void* payload);

void saved_stream_deinit(saved_stream_t* s);

int saved_stream_get_next(saved_stream_t* s);

size_t saved_stream_create_checkpoint_on_previous(saved_stream_t* s);

void saved_stream_load_checkpoint(saved_stream_t*s, size_t checkpoint);

#endif /* SAVED_STREAM_H */


