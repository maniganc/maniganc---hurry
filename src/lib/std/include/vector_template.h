

#define CLASS vector
#include "class_template.h"

#include <string.h>

typedef struct {
  TYPE* data_begin;
  TYPE* data_end_used;
  TYPE* data_end_allocated;
}THIS_CLASS_TEMPLATE;

void THIS_METHOD_TEMPLATE(init)(THIS_CLASS_TEMPLATE* vector);
			       
void THIS_METHOD_TEMPLATE(resize)(THIS_CLASS_TEMPLATE* vector, size_t size);
				 
void THIS_METHOD_TEMPLATE(resize_allocated)(THIS_CLASS_TEMPLATE* vector,
					    size_t size);
				     
void THIS_METHOD_TEMPLATE(push)(THIS_CLASS_TEMPLATE* vector, TYPE el);
			       
size_t THIS_METHOD_TEMPLATE(get_size)(THIS_CLASS_TEMPLATE* vector);
				     
TYPE THIS_METHOD_TEMPLATE(get_idx)(THIS_CLASS_TEMPLATE* vector, size_t idx);

void THIS_METHOD_TEMPLATE(set_idx)(THIS_CLASS_TEMPLATE* vector,
				   size_t idx,
				   TYPE el);
				  
void THIS_METHOD_TEMPLATE(pop)(THIS_CLASS_TEMPLATE* vector, TYPE* el);
			      
void THIS_METHOD_TEMPLATE(erase)(THIS_CLASS_TEMPLATE* vector);
				
TYPE* THIS_METHOD_TEMPLATE(get_raw_ptr)(THIS_CLASS_TEMPLATE* vector);
				       
void THIS_METHOD_TEMPLATE(deinit)(THIS_CLASS_TEMPLATE* vector);
