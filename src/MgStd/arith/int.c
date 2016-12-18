
#include "buildin_procedures.h"

#include "MgList.h"
#include "MgIdentifier.h"
#include "MgInterpreter.h"

#include "MgInteger.h"

#include "MgUnspecified.h"

#include "MgBool.h"

MG_BUILDIN_PROCEDURE(arith_add, "+") {
  MgStatus* s;

  int acc = 0;

  while ( (MgList*)arg != Mg_emptyList ) {
    MgObject* argn = MgList_get_car((MgList*)arg);
    MgInteger* arg_in;
    s = MgObject_evaluate(argn, (MgObject**)&arg_in, interpreter, env);
    if ( s != Mg_ok ) goto error;

    MgObject_add_reference((MgObject*)arg_in);

    if ( !Mg_is_an_integer((MgObject*)arg_in) ) {
      MgObject_drop_reference((MgObject*)arg_in);
      static const MgStatus err = { "+: not an integer" };
      return &err;
    }

    acc += MgInteger_get_value(arg_in);

    MgObject_drop_reference((MgObject*)arg_in);

    arg = MgList_get_cdr((MgList*)arg);
  }

  MgInteger* in;
  s = MgInteger_create_from_int(&in, acc);
  if ( s != Mg_ok ) goto error;

  *output = (MgObject*)in;
  return Mg_ok;

  error:
  return s;
}

MG_BUILDIN_PROCEDURE(arith_mult, "*") {
  MgStatus* s;

  int acc = 1;

  while ( (MgList*)arg != Mg_emptyList ) {
    MgObject* argn = MgList_get_car((MgList*)arg);
    MgInteger* arg_in;
    s = MgObject_evaluate(argn, (MgObject**)&arg_in, interpreter, env);
    if ( s != Mg_ok ) goto error;

    MgObject_add_reference((MgObject*)arg_in);

    if ( !Mg_is_an_integer((MgObject*)arg_in) ) {
      MgObject_drop_reference((MgObject*)arg_in);
      static const MgStatus err = { "*: not an integer" };
      return &err;
    }

    acc *= MgInteger_get_value(arg_in);

    MgObject_drop_reference((MgObject*)arg_in);

    arg = MgList_get_cdr((MgList*)arg);
  }

  MgInteger* in;
  s = MgInteger_create_from_int(&in, acc);
  if ( s != Mg_ok ) goto error;

  *output = (MgObject*)in;
  return Mg_ok;

  error:
  return s;
}

MG_BUILDIN_PROCEDURE(arith_sub, "-") {
  MgStatus* s;

  if ( (MgList*)arg == Mg_emptyList ) {
    static const MgStatus err = { "-: need at least one argument" };
    return &err;
  }

  MgObject* first_arg = MgList_get_car((MgList*)arg);
  MgInteger* first_in;
  s = MgObject_evaluate(first_arg, (MgObject**)&first_in, interpreter, env);
  if ( s != Mg_ok ) goto error;
  MgObject_add_reference((MgObject*)first_in);
  if ( !Mg_is_an_integer((MgObject*)first_in) ) {
    MgObject_drop_reference((MgObject*)first_in);
    static const MgStatus err = { "-: not an integer" };
    return &err;
  }

  int acc = MgInteger_get_value(first_in);

  MgObject_drop_reference((MgObject*)first_in);

  arg = MgList_get_cdr((MgList*)arg);
  if ( (MgList*)arg == Mg_emptyList ) {
    MgInteger* in;
    s = MgInteger_create_from_int(&in, -acc);
    if ( s != Mg_ok ) goto error;

    *output = (MgObject*)in;
    return Mg_ok;
  }

  while ( (MgList*)arg != Mg_emptyList ) {
    MgObject* argn = MgList_get_car((MgList*)arg);
    MgInteger* arg_in;
    s = MgObject_evaluate(argn, (MgObject**)&arg_in, interpreter, env);
    if ( s != Mg_ok ) goto error;

    MgObject_add_reference((MgObject*)arg_in);

    if ( !Mg_is_an_integer((MgObject*)arg_in) ) {
      MgObject_drop_reference((MgObject*)arg_in);
      static const MgStatus err = { "-: not an integer" };
      return &err;
    }

    acc -= MgInteger_get_value(arg_in);

    MgObject_drop_reference((MgObject*)arg_in);

    arg = MgList_get_cdr((MgList*)arg);
  }

  MgInteger* in;
  s = MgInteger_create_from_int(&in, acc);
  if ( s != Mg_ok ) goto error;

  *output = (MgObject*)in;
  return Mg_ok;

  error:
  return s;
}

MG_BUILDIN_PROCEDURE(arith_div, "/") {
  MgStatus* s;

  if ( (MgList*)arg == Mg_emptyList ) {
    static const MgStatus err = { "/: need at least one argument" };
    return &err;
  }

  MgObject* first_arg = MgList_get_car((MgList*)arg);
  MgInteger* first_in;
  s = MgObject_evaluate(first_arg, (MgObject**)&first_in, interpreter, env);
  if ( s != Mg_ok ) goto error;
  MgObject_add_reference((MgObject*)first_in);
  if ( !Mg_is_an_integer((MgObject*)first_in) ) {
    MgObject_drop_reference((MgObject*)first_in);
    static const MgStatus err = { "/: not an integer" };
    return &err;
  }

  int acc = MgInteger_get_value(first_in);

  MgObject_drop_reference((MgObject*)first_in);

  arg = MgList_get_cdr((MgList*)arg);
  if ( (MgList*)arg == Mg_emptyList ) {
    MgInteger* in;
    s = MgInteger_create_from_int(&in, acc);
    if ( s != Mg_ok ) goto error;

    *output = (MgObject*)in;
    return Mg_ok;
  }

  while ( (MgList*)arg != Mg_emptyList ) {
    MgObject* argn = MgList_get_car((MgList*)arg);
    MgInteger* arg_in;
    s = MgObject_evaluate(argn, (MgObject**)&arg_in, interpreter, env);
    if ( s != Mg_ok ) goto error;

    MgObject_add_reference((MgObject*)arg_in);

    if ( !Mg_is_an_integer((MgObject*)arg_in) ) {
      MgObject_drop_reference((MgObject*)arg_in);
      static const MgStatus err = { "/: not an integer" };
      return &err;
    }

    int div = MgInteger_get_value(arg_in);
    if ( div == 0) {
      MgObject_drop_reference((MgObject*)arg_in);
      *output = (MgObject*)Mg_unspecified;
      return Mg_ok;
    }

    acc /= MgInteger_get_value(arg_in);

    MgObject_drop_reference((MgObject*)arg_in);

    arg = MgList_get_cdr((MgList*)arg);
  }

  MgInteger* in;
  s = MgInteger_create_from_int(&in, acc);
  if ( s != Mg_ok ) goto error;

  *output = (MgObject*)in;
  return Mg_ok;

  error:
  return s;
}
MG_BUILDIN_PROCEDURE(arith_quotient, "quotient") {
  MgStatus* s;

  if ( (MgList*)arg == Mg_emptyList ) {
    static const MgStatus err = { "/: need at least one argument" };
    return &err;
  }

  MgObject* first_arg = MgList_get_car((MgList*)arg);
  MgInteger* first_in;
  s = MgObject_evaluate(first_arg, (MgObject**)&first_in, interpreter, env);
  if ( s != Mg_ok ) goto error;
  MgObject_add_reference((MgObject*)first_in);
  if ( !Mg_is_an_integer((MgObject*)first_in) ) {
    MgObject_drop_reference((MgObject*)first_in);
    static const MgStatus err = { "/: not an integer" };
    return &err;
  }

  int acc = MgInteger_get_value(first_in);

  MgObject_drop_reference((MgObject*)first_in);

  arg = MgList_get_cdr((MgList*)arg);
  if ( (MgList*)arg == Mg_emptyList ) {
    MgInteger* in;
    s = MgInteger_create_from_int(&in, acc);
    if ( s != Mg_ok ) goto error;

    *output = (MgObject*)in;
    return Mg_ok;
  }

  while ( (MgList*)arg != Mg_emptyList ) {
    MgObject* argn = MgList_get_car((MgList*)arg);
    MgInteger* arg_in;
    s = MgObject_evaluate(argn, (MgObject**)&arg_in, interpreter, env);
    if ( s != Mg_ok ) goto error;

    MgObject_add_reference((MgObject*)arg_in);

    if ( !Mg_is_an_integer((MgObject*)arg_in) ) {
      MgObject_drop_reference((MgObject*)arg_in);
      static const MgStatus err = { "/: not an integer" };
      return &err;
    }

    int div = MgInteger_get_value(arg_in);
    if ( div == 0) {
      MgObject_drop_reference((MgObject*)arg_in);
      *output = (MgObject*)Mg_unspecified;
      return Mg_ok;
    }

    acc /= MgInteger_get_value(arg_in);

    MgObject_drop_reference((MgObject*)arg_in);

    arg = MgList_get_cdr((MgList*)arg);
  }

  MgInteger* in;
  s = MgInteger_create_from_int(&in, acc);
  if ( s != Mg_ok ) goto error;

  *output = (MgObject*)in;
  return Mg_ok;

  error:
  return s;
}

MG_BUILDIN_PROCEDURE(arith_eq, "=") {
  MgStatus* s;

  int val_cmp;
  int first = 1;

  while ( (MgList*)arg != Mg_emptyList ) {
    MgObject* argn = MgList_get_car((MgList*)arg);
    MgInteger* arg_in;
    s = MgObject_evaluate(argn, (MgObject**)&arg_in, interpreter, env);
    if ( s != Mg_ok ) goto error;

    MgObject_add_reference((MgObject*)arg_in);

    if ( !Mg_is_an_integer((MgObject*)arg_in) ) {
      MgObject_drop_reference((MgObject*)arg_in);
      static const MgStatus err = { "*: not an integer" };
      return &err;
    }

    int val = MgInteger_get_value(arg_in);
    
    if (first) {
      val_cmp = val;
      first = 0;
    }

    if (val_cmp != MgInteger_get_value(arg_in) ) {
      MgObject_drop_reference((MgObject*)arg_in);
      *output = (MgObject*)MgBool_false;
      return Mg_ok;
    }

    MgObject_drop_reference((MgObject*)arg_in);

    arg = MgList_get_cdr((MgList*)arg);
  }

  *output = (MgObject*)MgBool_true;
  return Mg_ok;

  error:
  return s;
}
MG_BUILDIN_PROCEDURE(arith_less, "<") {
  MgStatus* s;

  int val_cmp;
  int first = 1;

  while ( (MgList*)arg != Mg_emptyList ) {
    MgObject* argn = MgList_get_car((MgList*)arg);
    MgInteger* arg_in;
    s = MgObject_evaluate(argn, (MgObject**)&arg_in, interpreter, env);
    if ( s != Mg_ok ) goto error;

    MgObject_add_reference((MgObject*)arg_in);

    if ( !Mg_is_an_integer((MgObject*)arg_in) ) {
      MgObject_drop_reference((MgObject*)arg_in);
      static const MgStatus err = { "*: not an integer" };
      return &err;
    }

    int val = MgInteger_get_value(arg_in);
    
    if (first) {
      val_cmp = val;
      first = 0;
    }

    else if (val_cmp >= MgInteger_get_value(arg_in) ) {
      MgObject_drop_reference((MgObject*)arg_in);
      *output = (MgObject*)MgBool_false;
      return Mg_ok;
    }

    MgObject_drop_reference((MgObject*)arg_in);

    arg = MgList_get_cdr((MgList*)arg);
  }

  *output = (MgObject*)MgBool_true;
  return Mg_ok;

  error:
  return s;
}

MG_BUILDIN_PROCEDURE(arith_greater, ">") {
  MgStatus* s;

  int val_cmp;
  int first = 1;

  while ( (MgList*)arg != Mg_emptyList ) {
    MgObject* argn = MgList_get_car((MgList*)arg);
    MgInteger* arg_in;
    s = MgObject_evaluate(argn, (MgObject**)&arg_in, interpreter, env);
    if ( s != Mg_ok ) goto error;

    MgObject_add_reference((MgObject*)arg_in);

    if ( !Mg_is_an_integer((MgObject*)arg_in) ) {
      MgObject_drop_reference((MgObject*)arg_in);
      static const MgStatus err = { "*: not an integer" };
      return &err;
    }

    int val = MgInteger_get_value(arg_in);
    
    if (first) {
      val_cmp = val;
      first = 0;
    }

    else if (val_cmp <= MgInteger_get_value(arg_in) ) {
      MgObject_drop_reference((MgObject*)arg_in);
      *output = (MgObject*)MgBool_false;
      return Mg_ok;
    }

    MgObject_drop_reference((MgObject*)arg_in);

    arg = MgList_get_cdr((MgList*)arg);
  }

  *output = (MgObject*)MgBool_true;
  return Mg_ok;

  error:
  return s;
}
