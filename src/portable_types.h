/**
 * @file   portable_types.h
 * @author Adam Risi <ajrisi@gmail.com>
 * @date   Tue Mar 10 18:24:12 2009
 * 
 * @brief Portable types header file, defines the pack and unpack
 * routines for base types, and creates the portable_X packages for
 * passing to pack and unpack. Should not use any networking
 * libraries, for portability
 * 
 * 
 */

#ifndef _PORTABLE_TYPES_H_
#define _PORTABLE_TYPES_H_


/* This value can only be a power of 2 */
#define BYTES_PER_PORTABLE_DATA_UNIT 4

#define IS_LITTLE_ENDIAN ((char)(int)1)
#define IS_BIG_ENDIAN (!IS_LITTLE_ENDIAN)
#define PDSIZE(x)  (((x) + BYTES_PER_PORTABLE_DATA_UNIT - 1) & ~(BYTES_PER_PORTABLE_DATA_UNIT - 1))

#ifndef NULL
#define NULL ((void*)0)
#endif

typedef unsigned char byte;
typedef unsigned int psize_t;

typedef struct portable_type_s portable_type;
struct portable_type_s {
  int version_number;
  byte *(*pack)(void *src, byte *dst);
  void *(*unpack)(byte *src, void *dst);
  psize_t (*size)(void *src);
};

#define PORTABLE_INT_VERSION 0
byte *packInt(void *src, byte *dst);
void *unpackInt(byte *src, void *dst);
psize_t sizeInt(void *src);

static portable_type portable_int = {PORTABLE_INT_VERSION, packInt, unpackInt, sizeInt};
static portable_type portable_enum = {PORTABLE_INT_VERSION, packInt, unpackInt, sizeInt};
static portable_type portable_bool = {PORTABLE_INT_VERSION, packInt, unpackInt, sizeInt};

#define PORTABLE_HYPER_VERSION 0
byte *packHyper(void *src, byte *dst);
void *unpackHyper(byte *src, void *dst);
psize_t sizeHyper(void *src);

static portable_type portable_hyper = {PORTABLE_HYPER_VERSION, packHyper, unpackHyper, sizeHyper};

#define PORTABLE_FLOAT_VERSION 0

#define PORTABLE_LONG_FLOAT_VERSION 0

#define PORTABLE_OPAQUE_VERSION 0
typedef struct opaque_data_s opaque_data;
struct opaque_data_s {
  unsigned int size;
  byte *data;
};
byte *packOpaque(void *src, byte *dst);
void *unpackOpaque(byte *src, void *dst);
psize_t sizeOpaque(void *src);

static portable_type portable_opaque = {PORTABLE_OPAQUE_VERSION, packOpaque, unpackOpaque, sizeOpaque};

#define PORTABLE_STRING_VERSION 0
byte *packString(void *src, byte *dst);
void *unpackString(byte *src, void *dst);
psize_t sizeString(void *src);

static portable_type portable_string = {PORTABLE_STRING_VERSION, packString, unpackString, sizeString};






#endif /* _PORTABLE_TYPES_H_ */


