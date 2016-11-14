#ifndef MGINTERPRETER_H
#define MGINTERPRETER_H

#include "MgStatus.h"
#include "MgEnvironment.h"

typedef struct MgInterpreter MgInterpreter;

/**
 * create a maniganc interpreter
 *
 * @param interpreter
 * @return
 */
MgStatus* MgInterpreter_create(MgInterpreter** interpreter);


/**
 * destroy a maniganc interpreter
 *
 * @param interpreter
 * @return
 */
MgStatus* MgInterpreter_destroy(MgInterpreter* interpreter);

/**
 * parse and run code from a saved stream
 *
 * @param interpreter
 * @param ss
 * @param interactive_mode null if not interactive
 * @return
 */
MgStatus* MgInterpreter_evaluate_sstream(MgInterpreter* interpreter,
                                         MgSavedStream* ss,
                                         int interactive_mode,
                                         int parse_only_mode);

/**
 * parse and run code from a filestream
 *
 * @param interpreter
 * @param fs
 * @param interactive_mode null if not interactive
 * @return
 */
MgStatus* MgInterpreter_evaluate_stream(MgInterpreter* interpreter,
                                        FILE* fs,
                                        int interactive_mode,
                                        int parse_only_mode);

/**
 * parse and run code from a file
 *
 * @param interpreter
 * @param filepath
 * @return
 */
MgStatus* MgInterpreter_evaluate_file(MgInterpreter* interpreter,
                                      const char* filepath);


/**
 * parse and run code from a string
 *
 * @param interpreter
 * @param str zero-terminated string
 * @return
 */
MgStatus* MgInterpreter_evaluate_string(MgInterpreter* interpreter,
                                        const char* str);

/**
 * get the symbol environment of an interpreter
 *
 * @param interpreter
 * @return env
 */
MgEnv* MgInterpreter_get_symbol_environment(const MgInterpreter* interpreter);

MgList* MgInterpreter_get_emptylist(const MgInterpreter* interpreter);

#endif /* MGINTERPRETER_H */
