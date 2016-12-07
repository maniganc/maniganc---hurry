#ifndef MGSTRING_H
#define MGSTRING_H


#include "MgParser.h"

typedef struct MgString MgString;

MgStatus* MgString_parser_func(MgSavedStream* ss,
			     const  MgObjectParser** object_parsers,
			     MgString** output_string);

extern const MgObjectParser MgString_parser;

MgStatus* MgString_create_from_string(MgString** string, char* str);

int Mg_is_a_string(const MgObject* obj);

const char* MgString_get_string(const MgString* string);

#endif /* MGSTRING_H */
