#ifndef MGCHAR_H
#define MGCHAR_H

#include "MgParser.h"

typedef struct MgChar MgChar;

MgStatus* MgChar_parser_func(MgSavedStream* ss,
			     const  MgObjectParser** object_parsers,
			     MgChar** output_char);

extern const MgObjectParser MgChar_parser;

int Mg_is_an_char(MgObject* obj);

MgStatus* MgChar_create_(MgChar** ch, char c);


#endif /* MGCHAR_H */
