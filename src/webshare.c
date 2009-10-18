/**
 * @file   webshare.c
 * @author Adam Risi
 * @date   Tue Feb 10 2009
 * 
 * @brief  The main webshare source file, a better description should go here
 * 
 * 
 */

#include <string.h>

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>


#include "config.h"
#include "version.h"

#include "webshare.h"
#include "wserror.h"
#include "http.h"
#include "mime.h"
#include "wspagetable.h"

/**
 * Initialize the webshare system - this does not actually start
 * anything, just initialize the internal storage
 *
 * @param  
 * @return 
 */
WEBSHARE *webshare_init(webshare_config *config)
{
  WEBSHARE *ws = (WEBSHARE*)malloc(sizeof(WEBSHARE));
  if(ws == NULL) return NULL;
  if(config != NULL) {
    //customized configuration
    memcpy(&ws->config, config, sizeof(webshare_config));
  } else {
    //default configuration
    ws->config = webshare_default_config();
  }

  /* initialize the page table */
  if(webpage_table_init(&(ws->pagetable)) != 0) {
    wserror(ws, FATAL, "could not initialize the webpage table\n");
  }

  /* initialize the mime table */
  if(mime_trie == NULL)
    mime_table_init();

  http_init();

  return ws;
}

webshare_config webshare_default_config()
{
  webshare_config ws = {0};
  ws.host = DEFAULT_HOST;
  ws.port = DEFAULT_PORT;
  
  ws.mime.unknown = UNKNOWN_MIME;
  
  ws.threading.model = THREAD_PER_CLIENT;
  ws.threading.pool_size = THREAD_POOL_SIZE;
  
  return ws;
}

/** 
 * Start up the webshare server. This function should not return 
 * UNTIL it has been directed to by a call to the stop function (from a seperate thread is fine)
 * 
 * @param ws 
 * 
 * @return 
 */
int webshare_start(WEBSHARE *ws)
{
  int listenfd, connfd;
  int i, maxi;
  int nready;
  struct sockaddr_in servaddr;

  struct sockaddr client;
  socklen_t client_size;


  listenfd = socket(PF_INET, SOCK_STREAM, 0);
  if(listenfd == -1) {
    return -1;
  }

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(9899);

  if(bind(listenfd, (struct sockaddr *)&servaddr, (socklen_t)sizeof(servaddr)) == -1) {
    return -1;
  }

  if(listen(listenfd, 5) == -1) {
    return -1;
  }

  ws->listenfd = listenfd;
  
  char yep = 1;
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yep, sizeof(char));  
  while((connfd = accept(listenfd, &client, &client_size)) != -1) {
    setsockopt(connfd, SOL_SOCKET, SO_REUSEADDR, &yep, sizeof(char));
    //read and toss from connfd
    char data_read[HTTP_MAX_REQUEST_HEADER_SIZE] = {0};
    while(read(connfd, data_read, HTTP_MAX_REQUEST_HEADER_SIZE) != 0) {
      //printf("%s\n", data_read);
      if(strstr(data_read, "\r\n\r\n") != NULL) {
	break;
      }
    }
    printf("read %d bytes\n", strlen(data_read));
    printf("Done read.\n");
    
    /* ideally, parse the headers, look for the url, and do the app. sending */

    http_request req = parse_http_request(data_read);

    /* look at the path, see if there is something lined up to serve */
    webpage_table_item *it = webpage_table_find(ws->pagetable, req.path);
    if(it != NULL) {
      if(it->postitem_type == POSTITEM_BUFFER) {
	/* compare the types of shares, if it is a buffer, send it out */
	char *test_page = "HTTP/1.0 200 OK\r\nContent-type: text/plain\r\n\r\n";
	write(connfd, test_page, strlen(test_page));
	write(connfd, it->value.buffer, strlen(it->value.buffer));
      } else if(it->postitem_type == POSTITEM_FILE) {
	/* if it is a file, send out the header, then read in and send out the file */
	char header[1024];
	char *header_fmt = "HTTP/1.0 200 OK\r\nContent-type: %s\r\n\r\n";
	sprintf(header, header_fmt, it->
      }
    } else {
      char *test_page = "HTTP/1.0 404 Page Not Found\r\nContent-type: text/plain\r\n\r\n404 Page Not Found";
      write(connfd, test_page, strlen(test_page));
    }

    close(connfd);
  }

}

/** 
 * Stop the webshare server
 * 
 * @param ws 
 * 
 * @return 
 */
int webshare_stop(WEBSHARE *ws)
{
  close(ws->listenfd);

  webshare_pagetable_free(ws);

  http_free();
}

