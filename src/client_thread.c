#include <client_thread.h>
#include "pagetable.h"
#include "http.h"

void *client_thread(void *data)
{
  http_request r;
  client_data *cd;
  char buffer[MAX_REQUEST_HEADER_LENGTH];
  int rc;
  webpage_table_item *page;

  cd = (client_data*)data;
  memset(&r, 0, sizeof(r));
  
  /* read from the remote machine to get the method and URL */
  rc = read(cd->remote_fd, buffer, MAX_REQUEST_HEADER_LENGTH); 
  if(rc == -1) {
    /* was unable to read from the remote FD, so quit */
    return NULL;
  }

  /* parse the request */
  rc = http_request_parse(&r, buffer, MAX_REQUEST_HEADER_LENGTH);
  if(rc < 0) {
    /* unable to parse the http request */
    send_error(cd->remote_fd, 400, "Bad Request");
    return NULL;
  }

  printf("URL: %s\n", r.url);
 
  /* now, lookup the URL, and do the app. reply */
  page = pagetable_find(cd->ws->pages, r.url);
  if(page == NULL) {
    /* page not found, 404 error */
    send_error(cd->remote_fd, 404, "File Not Found");
  } else {
    /* page found, switch on type, then do */
    switch(page->postitem_type) {
    case POSTITEM_BUFFER:
      send_error(cd->remote_fd, 200, page->value.buffer);
      break;
    defualt:
      send_error(cd->remote_fd, 501, "Post Type not Implemented");
      break;
    }
  }

  /* shutdown the request */
  close(cd->remote_fd);

  return NULL;
}

