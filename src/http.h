#ifndef HTTP_H
#define HTTP_H

#include <webshare.h>

#define MAX_URL_LENGTH 1024
#define MAX_METHOD_LENGTH 16
#define MAX_REQUEST_HEADER_LENGTH 2048
#define MAX_REPLY_HEADER_LENGTH 2048

typedef enum http_method_e http_method;
enum http_method_e {
  OPTIONS,	GET,	HEAD,	POST,
  PUT, 		DELETE,	TRACE,	CONNECT,
  HTTP_ERR=-1
};

typedef struct http_request_s http_request;
struct http_request_s {
  http_method method;
  char url[MAX_URL_LENGTH];
};

typedef struct http_reply_s http_reply;
struct http_reply_s {
  int code;
  char *message;
  unsigned int size;
  char *mime;
  
};

int http_request_parse(http_request *r, char *buffer, unsigned int blen);
int send_error(int fd, int code, char *desc);

#endif
