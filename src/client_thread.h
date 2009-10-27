#ifndef CLIENT_THREAD_H
#define CLIENT_THREAD_H

#include <webshare.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct client_data_s client_data;
struct client_data_s {
  WEBSHARE *ws;
  int remote_fd;
};

/** 
 * This is the client handling routine, it is passed a client_data
 * type address, and it will read from the remote_fd, parse the
 * request, reply, and close the connection
 * 
 * @param data a client_data pointer
 * 
 * @return 
 */
void *client_thread(void *data);

#endif

