#ifndef MGBOOL_H
#define MGBOOL_H

#include "MgParser.h"

typedef struct MgBool MgBool;

extern MgBool* MgBool_true;
extern MgBool* MgBool_false;

MgStatus* MgBool_parser_func(MgSavedStream* ss,
			     const  MgObjectParser** object_parsers,
			     MgBool** output_bool);

extern const MgObjectParser MgBool_parser;

int Mg_is_a_bool(MgObject* obj);

int Mg_is_true(MgObject* obj);

int Mg_is_false(MgObject* obj);

#endif /* MGBOOL_H */
