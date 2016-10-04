#ifndef MGIDENTIFIER_H
#define MGIDENTIFIER_H

#include "MgParser.h"

typedef struct MgIdentifier MgIdentifier;

extern const MgObjectType* MgIdentifier_type;

MgStatus* MgIdentifier_parser_func(MgSavedStream* ss,
			     const  MgObjectParser** object_parsers,
			     MgIdentifier** output_identifier);

extern const MgObjectParser MgIdentifier_parser;

MgStatus* MgIdentifier_create_from_string(MgIdentifier** identifier, char* name);

int Mg_is_an_identifier(MgObject* obj);

const char* MgIdentifier_get_name(const MgIdentifier* identifier);

#endif /* MGIDENTIFIER_H */
