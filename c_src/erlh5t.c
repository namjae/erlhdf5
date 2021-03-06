/* Copyright (C) 2012 Roman Shestakov */

/* This file is part of erlhdf5 */

/* elrhdf5 is free software: you can redistribute it and/or modify */
/* it under the terms of the GNU Lesser General Public License as */
/* published by the Free Software Foundation, either version 3 of */
/* the License, or (at your option) any later version. */

/* elrhdf5 is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/* GNU Lesser General Public License for more details. */

/* You should have received a copy of the GNU Lesser General Public */
/* License along with Erlsom.  If not, see */
/* <http://www.gnu.org/licenses/>. */

/* Author contact: romanshestakov@yahoo.co.uk */

#include <stdio.h>
#include <stdlib.h>
#include "hdf5.h"
#include "erl_nif.h"
#include "dbg.h"
#include "erlhdf5.h"

// H5T: Datatype Interface

// protype
static int convert_type(char*,  hid_t*);

// convert
static int convert_type(char* dtype,  hid_t* dtype_id)
{
  if(strncmp(dtype, "H5T_NATIVE_INT", MAXBUFLEN) == 0)
    *dtype_id = H5T_NATIVE_INT;
  else
    sentinel("Unknown type %s", dtype);
  return 0;

 error:
  return -1;
};


// Creates a new property list as an instance of a property list class.
ERL_NIF_TERM h5tcopy(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
  hid_t dtype_id; // dataset creation property list
  hid_t type_id;
  ERL_NIF_TERM ret;
  char type[MAXBUFLEN];

  // parse arguments
  check(argc == 1, "Incorrent number of arguments");
  check(enif_get_atom(env, argv[0], type, sizeof(type), ERL_NIF_LATIN1), \
	"Can't get type from argv");

  // convert type to format which hdf5 api understand
  check(!convert_type(type, &dtype_id), "Failed to convert type");

  type_id = H5Tcopy(dtype_id);
  check(type_id > 0, "Failed to create type.");

  ret = enif_make_int(env, type_id);
  return enif_make_tuple2(env, ATOM_OK, ret);

 error:
  if(type_id) H5Tclose(type_id);
  return error_tuple(env, "Can not copy type");
};


// close
ERL_NIF_TERM h5tclose(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
  hid_t type_id;

  // parse arguments
  check(argc == 1, "Incorrent number of arguments");
  check(enif_get_int(env, argv[0], &type_id), "Can't get resource from argv");

  // close properties list
  check(!H5Tclose(type_id), "Failed to close type.");
  return ATOM_OK;

 error:
  return error_tuple(env, "Can not close type");
};


// Returns the datatype class identifier.
ERL_NIF_TERM h5tget_class(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
  ERL_NIF_TERM ret;
  H5T_class_t class_id;
  hid_t type_id;

  // parse arguments
  check(argc == 1, "Incorrent number of arguments");
  check(enif_get_int(env, argv[0], &type_id), "Can't get resource from argv");

  class_id = H5Tget_class(type_id);
  //fprintf(stderr, "class type: %d\r\n", class_id);
  check(class_id != H5T_NO_CLASS, "Failed to get type class.");

  ret = enif_make_int(env, class_id);
  return enif_make_tuple2(env, ATOM_OK, ret);

 error:
  return error_tuple(env, "Can not get type class");
};


// Returns the byte order of an atomic datatype.
ERL_NIF_TERM h5tget_order(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
  ERL_NIF_TERM ret;
  H5T_order_t order;
  hid_t type_id;

  // parse arguments
  check(argc == 1, "Incorrent number of arguments");
  check(enif_get_int(env, argv[0], &type_id), "Can't get resource from argv");

  order = H5Tget_order(type_id);
  check(order != H5T_ORDER_ERROR, "Failed to get order.");

  ret = enif_make_int(env, order);
  return enif_make_tuple2(env, ATOM_OK, ret);

 error:
  return error_tuple(env, "Can not get order");
};


// Returns the byte order of an atomic datatype.
ERL_NIF_TERM h5tget_size(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
  ERL_NIF_TERM ret;
  size_t size;
  hid_t type_id;

  // parse arguments
  check(argc == 1, "Incorrent number of arguments");
  check(enif_get_int(env, argv[0], &type_id), "Can't get resource from argv");

  size = H5Tget_size(type_id);
  check(size > 0, "Failed to get size.");

  ret = enif_make_int(env, size);
  return enif_make_tuple2(env, ATOM_OK, ret);

 error:
  return error_tuple(env, "Can not get size");
};
