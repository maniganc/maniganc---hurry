#include "buildin_procedures.h"
const Mg_buildin_procedure Mg_buildin_procedure_array[] = {
{mg_buildin_procedure__refcnt, "_refcnt"},
{mg_buildin_procedure__env, "_env"},
{mg_buildin_procedure_define, "define"},
{mg_buildin_procedure_if, "if"},
{mg_buildin_procedure_pp, "pp"},
{mg_buildin_procedure_quote, "quote"},
{mg_buildin_procedure_set_mutable, "set!"},
{NULL, NULL}};
