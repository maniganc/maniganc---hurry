#ifndef MGQUOTE_H
#define MGQUOTE_H

#include "MgParser.h"
#include "MgList.h"

MgStatus* MgQuote_parser_func(MgSavedStream* ss,
			     const  MgObjectParser** object_parsers,
			     MgList** output_list);

extern const MgObjectParser MgQuote_parser;

int Mg_is_quote(MgObject* obj);

#endif /* MGQUOTE_H */
