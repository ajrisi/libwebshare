#include <glib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#include "http.h"

/* cpt.h */

#ifndef TORUS_CPT_H
#define TORUS_CPT_H

struct cpt_data {
  //socket information
  int remote_fd;
  struct sockaddr_in remote_addr;
  GIOChannel *ioc;

  //header information
  GPtrArray *headers;
};

/*
void cpt_data_free(struct CPT_data*);
*/

extern GThreadPool *client_thread_pool;

void http_cpt_init(void);

void *http_cpt(void*, void*);

#endif
