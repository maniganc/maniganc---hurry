#ifndef MGNUMBER_H
#define MGNUMBER_H

#include "MgParser.h"

extern const MgObjectType* MgNumber_type;

typedef struct MgNumber MgNumber;


typedef MgStatus* (*MgNumber_represent_func)(MgNumber* self,
                                             FILE* fs);

typedef MgStatus* (*MgNumber_destroy_func)(MgNumber* self);


typedef MgStatus* (*MgNumber_is_equal_func)(MgNumber* self,
                                            MgNumber* other);

typedef MgStatus* (*MgNumber_add_func)(MgNumber* self,
                                       MgNumber* other,
                                       MgNumber** output);

typedef MgStatus* (*MgNumber_sub_func)(MgNumber* self,
                                       MgNumber* other,
                                       MgNumber** output);

typedef MgStatus* (*MgNumber_mult_func)(MgNumber* self,
                                       MgNumber* other,
                                       MgNumber** output);

typedef MgStatus* (*MgNumber_div_func)(MgNumber* self,
                                       MgNumber* other,
                                       MgNumber** output);

typedef struct {
  MgNumber_represent_func represent;
  MgNumber_destroy_func destroy;

  MgNumber_is_equal_func is_equal;
  MgNumber_add_func add;
  MgNumber_sub_func sub;
  MgNumber_mult_func mult;
  MgNumber_div_func div;
}MgNumberType;

struct MgNumber { MgObject base;
  MgNumberType type;
};



int Mg_is_a_number(const MgObject* obj);

#endif /* MGNUMBER_H */
