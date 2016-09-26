#ifndef MGSTRING_H
#define MGSTRING_H


#include "MgParser.h"

typedef struct MgString MgString;

MgStatus* MgString_parser_func(MgSavedStream* ss,
			     const  MgObjectParser** object_parsers,
			     MgString** output_string);

extern const MgObjectParser MgString_parser;


#endif /* MGSTRING_H */
