#ifndef MGPARSER_H
#define MGPARSER_H

#include "MgObject.h"
#include "MgSavedStream.h"

typedef struct MgObjectParser MgObjectParser;

typedef MgStatus* (*MgObjectParser_func)(MgSavedStream* ss,
                                         const MgObjectParser** object_parsers,
                                         MgObject** output_object);

struct MgObjectParser {
  MgObjectParser_func parse;
};

/**
 * skip line
 *
 * @param ss 
 * @return Mg_ok if not errors
 */
MgStatus* MgParser_skip_line(MgSavedStream* ss);
/**
 * skip whitespaces and comments. 
 * and count whitespaces
 *
 * @param ss 
 * @param whitespace_cnt if not NULL, whitespaces count is returned
 * @return Mg_ok if no errors
 */
MgStatus* MgParser_skip_sugar(MgSavedStream* ss, int* whitespace_cnt);

/**
 * skip sugar (whitespaces & comments), parse stream and output the first object
 *
 * @param ss 
 * @param object_parsers null terminated array of parsers
 * @param output_object 
 * @return Mg_ok if no errors
 */
MgStatus* MgParser_parse(MgSavedStream* ss,
                         const MgObjectParser** object_parsers,
                         MgObject** output_object);


/**
 * parse stream and output object. Each parser is tested until 
 * one successully parse an object.
 * output_object is given to the caller, thus it has to drop it 
 * after using it.
 *
 * @param ss 
 * @param object_parsers null terminated array of parsers pointers
 * @param output_object 
 * @return 
 */
MgStatus* MgParser_parse_object(MgSavedStream* ss,
                                const MgObjectParser** object_parsers,
                                MgObject** output_object);


/**
 * check symbol ending.
 * example: ...#tAnotherSymbol) might be incorrectly parsed
 * as #t and AnotherSymbol. 
 * this function solve that issue by searching for some sugar
 * or a closing paren after the symbol.
 * note: TODO: in the future maybe, allow other ending symbols (]}...)
 *
 * @param ss 
 * @return Mg_ok if ending is correct
 */
MgStatus* MgParser_check_symbol_ending(MgSavedStream* ss);

extern const MgStatus* MgParser_error_syntax;
extern const MgStatus* MgParser_error_getchar;

#endif /* MGPARSER_H */
