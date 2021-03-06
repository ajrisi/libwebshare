/**
 * @file   webshare.h
 * @author Adam Risi
 * @date   Tue Feb 10 2009
 * 
 * @brief  This file is simply an "include" collection of all of the other headers that
 *         are used as part of webshare
 * 
 * 
 */


#ifndef WEBSHARE_H
#define WEBSHARE_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

#include <config.h>
#include <version.h>

#include <http.h>
#include <string.h>
#include <util.h>

#include <pagetable.h>

typedef struct webshare_config_s webshare_config;
struct webshare_config_s {
  char *host;
  short port;
  struct {
    char *unknown;
  } mime;

  struct {
    int model;
    int pool_size;
  } threading;

  struct {
    void(*info_cb)(char *);
    void(*warning_cb)(char *);
    void(*critical_cb)(char *);
    void(*fatal_cb)(char *);
  } error_callbacks;
};

typedef struct webshare_s WEBSHARE;
struct webshare_s {
  int listenfd;
  pagetable *pages;
  webshare_config config;
};

enum {
  NO_THREADS,
  THREAD_PER_CLIENT,
  THREAD_POOL,
};

typedef enum {
  PI_BUFFER,
  PI_FUNCTION,
  PI_FILE
} post_type_t;

/** 
 * Create a new webshare embedded HTTP server
 * 
 * @param config 
 * 
 * @return 
 */
WEBSHARE *webshare_new(webshare_config *config);

/** 
 * Start the webshare server, this function will NOT return
 * 
 * @param ws the webshare to start
 * 
 * @return 0 on success, neg, on failure
 */
int webshare_start(WEBSHARE *ws);

/** 
 * Stop the webshare server
 * 
 * @param ws the server to stop
 * 
 * @return 0 on success, neg. on failure
 */
int webshare_stop(WEBSHARE *ws);

int webshare_post_buffer(WEBSHARE *ws, char *path, char *buffer);

int webshare_post_file(WEBSHARE *ws, char *path, char *file_path);

int webshare_post_function(WEBSHARE *ws, char *path, void*(*callback)(void *data));

void webshare_post_remove(WEBSHARE *ws, char *path);



#endif
