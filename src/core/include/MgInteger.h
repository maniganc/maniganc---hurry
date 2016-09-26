#ifndef MGINTEGER_H
#define MGINTEGER_H

#include "MgParser.h"

typedef struct MgInteger MgInteger;

MgStatus* MgInteger_parser_func(MgSavedStream* ss,
			     const  MgObjectParser** object_parsers,
			     MgInteger** output_integer);

extern const MgObjectParser MgInteger_parser;

int Mg_is_an_integer(MgObject* obj);

MgStatus* MgInteger_create_from_int(MgInteger** integer, int value);

#endif /* MGINTEGER_H */
