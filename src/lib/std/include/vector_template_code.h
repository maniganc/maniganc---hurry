
#include "vector_template.h"

#include <stdlib.h>
#include <string.h>

#include "debug.h"

void THIS_METHOD_TEMPLATE(init)(THIS_CLASS_TEMPLATE* vector) {
  vector->data_begin = 0;
  vector->data_end_used = 0;
  vector->data_end_allocated = 0;
}

void THIS_METHOD_TEMPLATE(resize)(THIS_CLASS_TEMPLATE* vector,
                                  size_t new_data_size) {
  THIS_METHOD_TEMPLATE(resize_allocated)(vector, new_data_size);
  vector->data_end_used = vector->data_end_allocated;
}

void THIS_METHOD_TEMPLATE(resize_allocated)(THIS_CLASS_TEMPLATE* vector,
                                            size_t new_size_allocated) {
  size_t size_used = vector->data_end_used - vector->data_begin;
  size_t size_to_copy = size_used < new_size_allocated ?
    size_used : new_size_allocated;
  TYPE* new_data_begin = malloc(new_size_allocated * sizeof(TYPE));
  if ( !new_data_begin ) {
    error("malloc failed\n");
  }
  if (vector->data_begin != NULL) {
    memcpy(new_data_begin, vector->data_begin, size_to_copy * sizeof(TYPE));
    free(vector->data_begin);
  }
  vector->data_begin = new_data_begin;
  vector->data_end_used = new_data_begin + size_to_copy;
  vector->data_end_allocated = new_data_begin + new_size_allocated;
}

void THIS_METHOD_TEMPLATE(push)(THIS_CLASS_TEMPLATE* vector, TYPE el) {
  if ( vector->data_end_used == vector->data_end_allocated ) {
    size_t size_allocated = vector->data_end_allocated - vector->data_begin;
    size_t next_size_allocated = size_allocated == 0 ? 1 : size_allocated * 2;
    THIS_METHOD_TEMPLATE(resize_allocated)(vector, next_size_allocated);
  }
  *(vector->data_end_used) = el;
  vector->data_end_used++;
}

TYPE THIS_METHOD_TEMPLATE(get_idx)(THIS_CLASS_TEMPLATE* vector, size_t idx) {
  return vector->data_begin[idx];
}

void THIS_METHOD_TEMPLATE(set_idx)(THIS_CLASS_TEMPLATE* vector,
                                   size_t idx,
                                   TYPE el) {
  vector->data_begin[idx] = el;
}

size_t THIS_METHOD_TEMPLATE(get_size)(THIS_CLASS_TEMPLATE* vector) {
  return vector->data_end_used - vector->data_begin;
}

void THIS_METHOD_TEMPLATE(pop)(THIS_CLASS_TEMPLATE* vector, TYPE* el) {
  if ( vector->data_end_used == vector->data_begin ) {
    error("underflow\n");
  }
  vector->data_end_used--;
  *el = *(vector->data_end_used);
}

void THIS_METHOD_TEMPLATE(erase)(THIS_CLASS_TEMPLATE* vector) {
  vector->data_end_used = vector->data_begin;
}

TYPE* THIS_METHOD_TEMPLATE(get_raw_ptr)(THIS_CLASS_TEMPLATE* vector) {
  return vector->data_begin;
}

void THIS_METHOD_TEMPLATE(deinit)(THIS_CLASS_TEMPLATE* vector) {
  if (vector->data_begin != NULL) {
    free(vector->data_begin);
  }
}
