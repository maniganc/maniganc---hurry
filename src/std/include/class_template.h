
#include "template.h"

#ifndef TYPE
#error TYPE macro symbol is not defined
#endif

#ifndef CLASS
#error CLASS macro symbol is not defined
#endif

#define THIS_CLASS_TEMPLATE CLASS_TEMPLATE(CLASS, TYPE)
#define THIS_METHOD_TEMPLATE(m) METHOD_TEMPLATE(CLASS, TYPE, m)

