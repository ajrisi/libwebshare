#ifndef _WSPARSE_HEADERS_H_
#define _WSPARSE_HEADERS_H_

enum {
  MOPTIONS, MGET, MHEAD, MPOST,
  MPUT, MDELETE, MTRACE, MCONNECT,
  MUNKNOWN
};

char* wshttp_methods[] = {
  "OPTIONS", "GET", "HEAD", "POST",	
  "PUT", "DELETE", "TRACE", "CONNECT"
};

typedef struct webshare_http_request_s webshare_http_request;

struct webshare_http_request_s {
  char data[65536];
  int method;
  char path[1024];
  char *version[16];
  char *rest;
};

#endif
