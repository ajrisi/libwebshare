/**
 * @file   wsparse_headers.c
 * @author adam <adam@chronos.rh.rit.edu>
 * @date   Mon Feb 16 00:04:55 2009
 * 
 * @brief  Parses recieved headers in an http request
 * 
 * 
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "wsparse_headers.h"

int wsparse_headers_method(char *method) {
  int i = 0;
  for(i=0; i < MUNKNOWN; i++)
    if(strcmp(wshttp_methods[i], method) == 0) return i;
  return MUNKNOWN;
}


webshare_http_request * wsparse_headers(char *raw_data, int data_len) {
  char *next_line = raw_data;
  char *method = malloc(8);

  /* first, scan off the first line and get the method, path, and the version info */
  printf("scanning %s\n", next_line);
  printf("scaned %d items\n", sscanf(next_line, "%8s %1024s %s16\r\n", method, r->path, r->version));
  if(sscanf(next_line, "%8s %1024s %16s\r\n", method, r->path, r->version) != 3) return 1;
  r->method = wsparse_headers_method(method);
  return 0;
}


#ifdef WSPARSEHEADERSTEST

int main(int argc, char **argv) {
  char *demo_header = "GET / HTTP/1.1\r\n\r\n";
  webshare_http_request *myrequest = malloc(sizeof(webshare_http_request));
  myrequest->path = malloc(1024);
  myrequest->version = malloc(16);

  if(wsparse_headers(myrequest, demo_header, strlen(demo_header)) == 0) {
    printf("successful parse\n");
    printf("method is %s\n", wshttp_methods[myrequest->method]);
    printf("path is %s\n", myrequest->path);
    printf("version is %s\n", myrequest->version);
  } else {
    fprintf(stderr, "error parsing demo headers\n");
  }
  
  return 0;
}

#endif
