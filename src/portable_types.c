/**
 * @file   portable_types.c
 * @author Adam Risi <ajrisi@gmail.com>
 * @date   Tue Mar 10 18:36:00 2009
 * 
 * @brief  Contains the pack and unpack routines for base types
 * 
 * 
 */

#include <string.h>
#include "portable_types.h"

byte *packInt(void *src, byte *dst)
{
  byte *data = (byte*)src;
  if(IS_LITTLE_ENDIAN) {
    dst[0] = data[3];
    dst[1] = data[2];
    dst[2] = data[1];
    dst[3] = data[0];
  } else {
    dst[0] = data[0];
    dst[1] = data[1];
    dst[2] = data[2];
    dst[3] = data[3];
  }
  return dst;
}


void *unpackInt(byte *src, void *dst)
{
  byte *data = (byte*)dst;
  if(IS_LITTLE_ENDIAN) {
    data[0] = src[3];
    data[1] = src[2];
    data[2] = src[1];
    data[3] = src[0];
  } else {
    data[0] = src[0];
    data[1] = src[1];
    data[2] = src[2];
    data[3] = src[3];
  }
  return dst;
}

psize_t sizeInt(void *src)
{
  return 4;
}

byte *packHyper(void *src, byte *dst)
{
  byte *data = (byte*)src;
  if(IS_LITTLE_ENDIAN) {
    dst[0] = data[7];
    dst[1] = data[6];
    dst[2] = data[5];
    dst[3] = data[4];
    dst[4] = data[3];
    dst[5] = data[2];
    dst[6] = data[1];
    dst[7] = data[0];
  } else {
    dst[0] = data[0];
    dst[1] = data[1];
    dst[2] = data[2];
    dst[3] = data[3];
    dst[4] = data[4];
    dst[5] = data[5];
    dst[6] = data[6];
    dst[7] = data[7];
  }
  return dst;
}


void *unpackHyper(byte *src, void *dst)
{
  byte *data = (byte*)dst;
  if(IS_LITTLE_ENDIAN) {
    data[0] = src[7];
    data[1] = src[6];
    data[2] = src[5];
    data[3] = src[4];
    data[4] = src[3];
    data[5] = src[2];
    data[6] = src[1];
    data[7] = src[0];
  } else {
    data[0] = src[0];
    data[1] = src[1];
    data[2] = src[2];
    data[3] = src[3];
    data[4] = src[4];
    data[5] = src[5];
    data[6] = src[6];
    data[7] = src[7];
  }
  return dst;
}

psize_t sizeHyper(void *src)
{
  return 8;
}

byte *packOpaque(void *src, byte *dst)
{
  byte *inptr = dst;
  opaque_data *od = (opaque_data*)src;
  portable_int.pack(&(od->size), inptr);
  inptr += portable_int.size(NULL);
  
  //move data from the opaque byte * to the portable structure.
  int i = 0;
  for(i=0; i < PDSIZE(od->size); i++) {
    if(i < od->size) {
      inptr[i] = od->data[i];
    } else {
      inptr[i] = 0;
    }
  }

  return dst;
}


void *unpackOpaque(byte *src, void *dst)
{
  opaque_data *od = (opaque_data*)dst;

  //read the size of the target opaque data first
  byte size_data[4] = {0};
  portable_int.unpack(src, size_data);
  unsigned int size = (unsigned int)*size_data;

  od->size = size;
  src += portable_int.size(NULL);

  //read "size" number of bytes from the structure into dst->data
  int i = 0;
  for(i = 0; i < size; i++) {
    od->data[i] = src[i];
  }

  return dst;
}

psize_t sizeOpaque(void *src)
{
  if(src == NULL) {
    return portable_int.size(NULL);
  }
  opaque_data *od = (opaque_data*)src;
  return portable_int.size(NULL) + PDSIZE(od->size);
}

byte *packString(void *src, byte *dst)
{
  opaque_data od = {0};
  char *str = (char*)src;

  /* add one here to send the null termination */
  od.size = strlen(str)+1;
  od.data = src;

  return packOpaque(&od, dst);
}

void *unpackString(byte *src, void *dst)
{
  opaque_data od = {0};
  od.data = (byte*)dst;
  unpackOpaque(src, &od);
  return dst;
}

psize_t sizeString(void *src)
{
  return portable_int.size(NULL) + PDSIZE(strlen((char*)src));
}

#ifdef PORTABLE_TYPES_TEST

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv) {
  printf("IS_LITTLE_ENDIAN: %c\n", IS_LITTLE_ENDIAN?'X':' ');
  printf("IS_BIG_ENDIAN:    %c\n", IS_BIG_ENDIAN?'X':' ');


  struct {
    struct portable_type_s t;
    int a;
    int b;
    struct {
      int c;
    };
  }d;
  d.t.size = sizeInt;
  
  //integer test, also suffices for enum and bool, what are really just integers
  int a = 256;
  byte packed[sizeof(int)] = {0};
  portable_int.pack(&a, packed);
  printf("Packed integer %d is %.2x %.2x %.2x %.2x\n", a, packed[0], packed[1], packed[2], packed[3]);
  printf("Unpacked, the integer is %d\n", *(int*)portable_int.unpack(packed, &a));

  long b = -256;
  byte packed_b[sizeof(long)] = {0};
  portable_hyper.pack(&b, packed_b);
  printf("type long is %d bytes\n", sizeof(long));
  printf("Packed long %d is  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x\n", b,
	 packed_b[0], packed_b[1], packed_b[2], packed_b[3], 
	 packed_b[4], packed_b[5], packed_b[6], packed_b[7]);
  
  
  //opaque_data test
  opaque_data od = {0};
  od.data = (byte*)"Hello, World!";
  od.size = strlen(od.data)+1;
  byte *packed_opaque = portable_opaque.pack(&od, malloc(portable_opaque.size(&od)));
  printf("Packed opaque data is as follows:\n");
  printf("Data Size: %.2x %.2x %.2x %.2x\n", packed_opaque[0], packed_opaque[1], packed_opaque[2], packed_opaque[3]);
  printf("Data     : %s\n", packed_opaque+4);
  free(packed_opaque);

  //string test
  byte *str_data =  portable_string.pack("Hello, World!", malloc(portable_string.size("Hello, World!")));
  printf("Packed string data is as follows:\n");
  printf("Data Size: %.2x %.2x %.2x %.2x\n", str_data[0], str_data[1], str_data[2], str_data[3]);
  printf("Data     : %s\n", str_data+4);
  free(str_data);
  
  return 0;
}

#endif /* PORTABLE_TYPES_TEST */
