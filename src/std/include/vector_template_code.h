
#include "vector_template.h"

#include <stdlib.h>
#include <string.h>

#include "debug.h"

void THIS_METHOD_TEMPLATE(init)(THIS_CLASS_TEMPLATE* vector) {
  vector->data_begin = 0;
  vector->data_end_used = 0;
  vector->data_end_allocated = 0;
}

void THIS_METHOD_TEMPLATE(resize)(THIS_CLASS_TEMPLATE* vector, size_t size) {
  size_t data_size_used = vector->data_end_used - vector->data_begin;
  size_t size_to_copy = data_size_used < size ? data_size_used : size;
  TYPE* data = malloc(size * sizeof(TYPE));
  if ( !data ) {
    error("malloc failed\n");
    exit(-1);
  }
  memcpy(data, vector->data_begin, size_to_copy * sizeof(TYPE));
  free(vector->data_begin);
  vector->data_begin = data;
  vector->data_end_allocated = data + size;
  vector->data_end_used = data + size;
}

void THIS_METHOD_TEMPLATE(resize_max)(THIS_CLASS_TEMPLATE* vector, size_t size) {
  size_t data_size_used = vector->data_end_used - vector->data_begin;
  size_t size_to_copy = data_size_used < size ? data_size_used : size;
  TYPE* data = malloc(size * sizeof(TYPE));
  if ( !data ) {
    error("malloc failed\n");
  }
  memcpy(data, vector->data_begin, size_to_copy * sizeof(TYPE));
  free(vector->data_begin);
  vector->data_begin = data;
  vector->data_end_allocated = data + size;
  vector->data_end_used = data + size_to_copy;
}

void THIS_METHOD_TEMPLATE(push)(THIS_CLASS_TEMPLATE* vector, TYPE el) {
  if ( vector->data_end_used == vector->data_end_allocated ) {
    size_t next_max_size = (vector->data_end_allocated - vector->data_begin +1)*2;
    THIS_METHOD_TEMPLATE(resize_max)(vector, next_max_size);
  }
  *(vector->data_end_used++) = el; 
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
  *el = *(--vector->data_end_used);
}

void THIS_METHOD_TEMPLATE(erase)(THIS_CLASS_TEMPLATE* vector) {
  vector->data_end_used = vector->data_begin;
}
			
TYPE* THIS_METHOD_TEMPLATE(get_raw_ptr)(THIS_CLASS_TEMPLATE* vector) {
  return vector->data_begin;
}

void THIS_METHOD_TEMPLATE(deinit)(THIS_CLASS_TEMPLATE* vector) {
  free(vector->data_begin);
}

void THIS_METHOD_TEMPLATE(print)
     (THIS_CLASS_TEMPLATE* vector,
      void (*FUNC_TEMPLATE(print, TYPE))(TYPE el)) {
  TYPE* iter = vector->data_begin;
  TYPE* iter_end = vector->data_end_used;
  printf("size used = %ld\n", vector->data_end_used - vector->data_begin);
  printf("size allocated = %ld\n", vector->data_end_allocated - vector->data_begin);
  int cnt = 0;
  while ( iter != iter_end ) {
    printf("[%d] = ",cnt++);
    FUNC_TEMPLATE(print, TYPE)(*iter);
    printf("\n");
    iter++;
  }
}
