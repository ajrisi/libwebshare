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



#include <sys/socket.h>
#include "hash.h"

#ifndef _WEBSHARE_H_
#define _WEBSHARE_H_

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
    void(*message_cb)(char *);
    void(*warning_cb)(char *);
    void(*critical_cb)(char *);
    void(*fatal_cb)(char *);
  } error_callbacks;
};

typedef struct webshare_s WEBSHARE;
struct webshare_s {
  int listenfd;
  webshare_config config;
  // webshare_error_callbacks error_callbacks;
  hshtbl *pagetable;
};

enum {
  THREAD_PER_CLIENT,
  THREAD_POOL,
  SELECT
};

typedef enum {
  PI_BUFFER,
  PI_FUNCTION,
  PI_FILE
} post_type_t;

/* Functions */
WEBSHARE *webshare_init(webshare_config *config);

int webshare_start(WEBSHARE *ws);
int webshare_stop(WEBSHARE *ws);

webshare_config webshare_default_config( void );


#endif
