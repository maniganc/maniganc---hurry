
#include "MgStatus.h"

static const MgStatus ok = {
  .message = "No Errors."
};
const MgStatus* Mg_ok = &ok;

static const MgStatus error = {
  .message = "Something something happened."
};
const MgStatus* Mg_error = &error;

static const MgStatus error_malloc = {
  .message = "memory allocation failed."
};
const MgStatus* Mg_error_malloc = &error_malloc;


static const MgStatus error_not_implemented = {
  .message = "not implemented feature."
};
const MgStatus* Mg_error_not_implemented = &error_not_implemented;

