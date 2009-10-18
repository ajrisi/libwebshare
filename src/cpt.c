#include <glib.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "cpt.h"
#include "callback_table.h"

/**
 * @file   cpt.c
 * @author adam <adam@chronos.rh.rit.edu>
 * @date   Sat Feb  7 20:39:50 2009
 * 
 * @brief This file contains all of the functions related to the
 *        client processing thread
 * 
 * 
 */

GThreadPool *client_thread_pool;

void http_cpt_init() {
  //TEMPORARY
  HTTP_SERVER_CLIENT_THREAD_POOL_SIZE = 20;
  //sanity checks
  g_assert(HTTP_SERVER_CLIENT_THREAD_POOL_SIZE != 0);

  //initialize the client thread pool
  GError *create_cpt_pool_err = 0;
  client_thread_pool = g_thread_pool_new(http_cpt, NULL, HTTP_SERVER_CLIENT_THREAD_POOL_SIZE, TRUE, &create_cpt_pool_err);
  if(create_cpt_pool_err != 0) {
    g_critical("Could not create the requested size client processing thread pool");
    exit(1);
  }

#ifdef DEBUG
  g_message("Created the CPT pool");
#endif
}

/** 
 * http_cpt is the function called when a thread is created to handle
 * a newly connected client. It has the responsibility to pass off
 * control to the first plugin possible.
 * 
 * @param my_cpt_data the cpt_data structure that contains the connection data
 * @param not_used not used
 * 
 * @return 
 */
void *http_cpt(void *my_cpt_data, void *not_used) {
  struct cpt_data *data = (struct cpt_data*)my_cpt_data;


  g_message("Created a CPT");
  g_message("Handling connection from IP %s", inet_ntoa(data->remote_addr.sin_addr));
  //try reading a line then scanning back to the beginning
  gchar *line;
  gchar *path;
  gsize length;
  int linenum = 0;
  //try reading the headers
  data->headers = g_ptr_array_new();

  while(g_io_channel_read_line(data->ioc, &line, &length, NULL, NULL) == G_IO_STATUS_NORMAL) {
    g_ptr_array_add(data->headers, strdup(line));
    g_message("got line %s", g_strescape(line, NULL));
    if(linenum == 0) {
      gchar **vec;
      vec = g_strsplit(line, " ", 3);
      g_message("target appears to be %s", vec[1]);
      path = g_strdup(vec[1]);
      g_strfreev(vec);
    }

    if(strcmp(line, "\r\n") == 0) {
      g_message("EOH");
      g_free(line);
      break;
    }
    linenum++;
    g_free(line);
  }
  g_message("done reading headers");

  //check callback registry for function that will handle this path.
  void* handle_func = http_callback_table_fetch(path);
  if(handle_func == NULL) {
    g_message("unknown requested path, returning 404");
    char *err404 = "HTTP/1.1 400 Bad Request\r\n"
                   "Content-Type: text/html\r\n\r\n"
      "<html><h1>404</h1></html>";
    g_io_channel_write_chars(data->ioc, err404, -1, NULL, NULL);
    g_io_channel_shutdown(data->ioc, TRUE, NULL);
  } else {
    g_message("successfully located function, calling");
    void*(*func)(void*) = handle_func;
    func((void*)data);
  }
  
  g_free(path);

  //the CPT must identify the module that will process this connection. The first way that a
  //module could be identified would be by the incoming IP address. 

  

  return (void*)0;
}
