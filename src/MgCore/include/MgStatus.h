#ifndef MGERROR_H
#define MGERROR_H

typedef const struct {
  char* message;
}MgStatus;

extern const MgStatus* Mg_ok;
extern const MgStatus* Mg_error;
extern const MgStatus* Mg_error_malloc;
extern const MgStatus* Mg_error_not_implemented;

#endif /* MGERROR_H */
