#ifndef HTTP_H
#define HTTP_H

#define _HTTP_LIB_VERSION_ 0.02

#define NUM_HTTP_HEADER_STRINGS 47
#define HTTP_HEADER_STRING_MALLOC 1024

/* GLOBAL VARIABLES */
/*
extern int HTTP_SERVER_PORT;
extern int HTTP_SERVER_MAIN_FD;
extern int HTTP_SERVER_ACCEPT_FD;
extern int HTTP_SERVER_CLIENT_THREAD_POOL_SIZE;
extern int HTTP_SERVER_REQUEST_THREAD_POOL_SIZE;
extern struct sockaddr_in HTTP_SERVER_LOCAL_ADDRESS;
*/

/*
typedef struct http_header_s http_header;

struct http_header_s {
  //in the case that the header has become invalid (and should be
  //ignored) then the type will be HTTP_ERR
  int type;
  int reply_type;
  char *first_line;

  int method;
  char *uri;
  char *http_version;

  char *header_lines[NUM_HTTP_HEADER_STRINGS];
};
*/

typedef struct _http_request http_request;
struct _http_request {
  int method;
  char *path;
  /* char *http_version; */
};

/*
enum {
  ACCEPT,               ACCEPT_CHARSET,                    ACCEPT_ENCODING,
  ACCEPT_LANGUAGE,      ACCEPT_RANGES,                     AGE,
  ALLOW,                AUTHORIZATION,                     CACHE_CONTROL,
  CONNECTION,           CONTENT_ENCODING,                  CONTENT_LANGUAGE,
  CONTENT_LENGTH,       CONTENT_LOCATION,                  CONTENT_SHA256,
  CONTENT_RANGE,        CONTENT_TYPE,                      DATE,
  ETAG,                 EXPECT,                            EXPIRES,
  FROM,                 HOST,                              IF_MATCH,
  IF_MODIFIED_SINCE,    IN_NONE_MATCH,                     IF_RANGE,
  IF_UNMODIFIED_SINCE,  LAST_MODIFIED,                     LOCATION,
  MAX_FORWARDS,         PRAGMA,                            PROXY_AUTHENTICATE,
  PROXY_AUTHENTICATION, RANGE,                             REFERER,
  RETRY_AFTER,          SERVER,                            TE,
  TRAILER,              TRANSFER_ENCODING,                 UPGRADE,
  USER_AGENT,           VARY,                              VIA,
  WARNING,              WWW_AUTHENTICATE
};

extern char* http_header_strings[];

enum {
	//Informational 1xx
	C100,	C101,
	//Successful 2xx
	C200,	C201,	C202,	C203,	C204,	C205,	C206,
	//Redirection 3xx
	C300,	C301,	C302,	C303,	C304,	C305,	C306,	C307,
	//Client Error 4xx
	C400,	C401,	C402,	C403,	C404,	C405,	C406,	C407,	C408,
	C409,	C410,	C411,	C412,	C413,	C414,	C415,	C416,	C417,
	//Server Error 5xx
	C500,	C501,	C502,	C503,	C504,	C505
       };

extern char* http_reply_codes[];
*/
enum {
  OPTIONS,	GET,	HEAD,	POST,
  PUT, 		DELETE,	TRACE,	CONNECT,
  HTTP_ERR=-1
};

typedef struct _charptr_int charptr_int;
struct _charptr_int {
  char *cptr;
  int i;
};

extern charptr_int http_methods[];

void http_init();
void http_free();

http_request parse_http_request(char *request_buffer);
static void parse_http_request_firstline(http_request *req, char *line);

#endif
