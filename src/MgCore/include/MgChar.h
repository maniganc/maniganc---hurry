#ifndef MGCHAR_H
#define MGCHAR_H

#include "MgParser.h"

typedef struct MgChar MgChar;

MgStatus* MgChar_parser_func(MgSavedStream* ss,
			     const  MgObjectParser** object_parsers,
			     MgChar** output_char);

extern const MgObjectParser MgChar_parser;

MgStatus* MgChar_create(MgChar** ch, char c);

int Mg_is_a_char(const MgObject* obj);

char MgChar_get_char(const MgChar* obj);

#endif /* MGCHAR_H */
