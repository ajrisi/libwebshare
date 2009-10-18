#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "http.h"
#include "trie.h"


int HTTP_SERVER_PORT;
int HTTP_SERVER_MAIN_FD;
int HTTP_SERVER_ACCEPT_FD;
int HTTP_SERVER_CLIENT_THREAD_POOL_SIZE;
int HTTP_SERVER_REQUEST_THREAD_POOL_SIZE;
struct sockaddr_in HTTP_SERVER_LOCAL_ADDRESS;

char* http_reply_codes[] = {
    "100 Continue\r\n",                           "101 Switching Protocols\r\n",
    "200 OK\r\n",                                 "201 Created\r\n",
    "202 Accepted\r\n",	                          "203 Non-Authoritave Information\r\n",
    "204 No Content\r\n",                         "205 Reset Content\r\n",
    "206 Partial Content\r\n",	                  "300 Multiple Choices\r\n",
    "301 Moved Permanently\r\n",                  "302 Found\r\n",
    "303 See Other\r\n",                          "304 Not Modified\r\n",
    "305 Use Proxy\r\n",                          "306 Unused\r\n",
    "307 Temporary Redirect\r\n",                 "400 Bad Request\r\n",
    "401 Unauthorized\r\n",                       "402 Payment Required\r\n",
    "403 Forbidden\r\n",                          "404 Not Found\r\n",
    "405 Method Not Allowed\r\n",                 "406 Not Acceptable\r\n",
    "407 Proxy Authentication Required\r\n",
    "408 Request Timeout\r\n",
    "409 Conflict\r\n",                           "410 Gone\r\n",
    "411 Length Required\r\n",                    "412 Precondition Failed\r\n",
    "413 Request Entity Too Large\r\n",           "414 Request URI Too Long\r\n",
    "415 Unsupported Media Type\r\n",
    "416 Requested Range Not Satisfiable\r\n",
    "417 Expectation Failed\r\n",                 "500 Internal Server Error\r\n",
    "501 Not Implemented\r\n",                    "502 Bad Gateway\r\n",
    "503 Service Unavailable\r\n",                "504 Gateway Timeout\r\n",
    "505 HTTP Version Not Supported\r\n"
};

char* http_header_strings[] = {
    "Accept",              "Accept-Charset",       "Accept-Encoding",
    "Accept-Language",     "Accept-Ranges",        "Age",
    "Allow",               "Authorization",        "Cache-Control",
    "Connection",          "Content-Encoding",     "Content-Language",
    "Content-Length",      "Content-Location",     "Content-SHA256",
    "Content-Range",       "Content-Type",         "Date",
    "ETag",                "Expect",               "Expires",
    "From",                "Host",                 "If-Match",
    "If-Modfied-Since",    "If-None-Match",        "If-Range",
    "If-Unmodified-Since", "Last-Modified",        "Location; ",
    "Max-Forwards",        "Pragma",               "Proxy-Authenticate",
    "Proxy-Authorization", "Range",                "Referer",
    "Retry-After",         "Server",               "TE",
    "Trailer",             "Transfer-Encoding",    "Upgrade",
    "User-Agent",          "Vary",                 "Via",
    "Warning",             "WWW-Authenticate"
};

charptr_int http_methods[] = {
  {"OPTIONS", OPTIONS},
  {"GET", GET},
  {"HEAD", HEAD},
  {"POST", POST},
  {"PUT", PUT},
  {"DELETE", DELETE},
  {"TRACE", TRACE},
  {"CONNECT", CONNECT},
  {NULL, -1}
};

static trie *http_method_trie;

void http_init()
{
  int i;
  http_method_trie = trie_new();
  
  for(i=0; http_methods[i].cptr != NULL; i++) {
    trie_insert(http_method_trie, http_methods[i].cptr, (void*)&http_methods[i].i);
  }
}

void http_free()
{
  trie_free(http_method_trie);
}

http_request parse_http_request(char *request_buffer)
{
  char *buffer_idx;
  char *current_line;
  int line_number;
  http_request req = {0};
  
  line_number = 0;
  buffer_idx = request_buffer;
  
  /* while loop for each line */
  while(buffer_idx != NULL) {
    /* line up next_line with the beginning of the next line */
    char *next_line = strpbrk(buffer_idx, "\r\n");
    if(next_line == NULL) {
      break;
    }
    while(*next_line == '\r' || *next_line == '\n') {
      *next_line = '\0';
      next_line++;
    }
    
    //fprintf(stderr, "Line::%s::\n", buffer_idx);
    if(line_number == 0) {
      /* special parsing for the first line */
      parse_http_request_firstline(&req, buffer_idx);
    } else {
      /* use regular parsing for the rest of the lines */
    }

    /* set buffer_idx to the beginning of the next line */
    buffer_idx = next_line;
    line_number++;
  }
  
  return req;
}

static void parse_http_request_firstline(http_request *req, char *line)
{
  int i;
  void *vptr;
  char *cptr;
  /* HARD CODED limit to a method length */
  char method[32] = {0};
  
  /* initialize the method, path, and version with error data */
  req->method = -1;
  req->path = NULL;
  /* req->http_version = NULL; */

  /* read in the method characters until a tab or space is located, then use the trie
     to match that with the app. number. If no match, use HTTP_ERR
  */
  for(i=0; (*line != '\t' && *line != ' ' && *line != '\0'); (line++, i++)) {
    method[i] = *line;
  }
  vptr = trie_lookup(http_method_trie, method);
  if(vptr != NULL) {
    req->method = *(int*)vptr;
  } else {
    /* unrecognized method */
    return;
  }

  /* skip tabs and spaces in line to get to the next field */
  while((*line == '\t' || *line == ' ') && (*line != '\0')) {
    line++;
  }

  /* get the number of characters until there is a tab, or space, or the end of the line, 
     and allocate that amount of space for the path  - there may be portability issues*/
  cptr = strpbrk(line, " \t");
  req->path = (char*)malloc(cptr-line+1);
  if(req->path == NULL) {
    printf("could not malloc for path, needed %d\n", cptr-line+1);
    return;
  }
  memset((void*)req->path, 0, cptr-line+1);
 
  /* copy over the path */
  strncpy(req->path, line, cptr-line);

  /* we dont really care about the http version, so it is ignored */
}

