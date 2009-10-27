/**
 * @file   wserror.c
 * @author Adam Risi <ajrisi@gmail.com>
 * @date   Mon Mar  2 07:22:48 2009
 * 
 * @brief  Error logging facilities
 * 
 * 
 */

#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "webshare.h"

#ifndef WSERROR_H
#define WSERROR_H

#include "wserror.h"

void wserror(WEBSHARE *share, int error_type, char *fmt, ...)
{
  char prefixbuffer[256] = {0};
  char linebuffer[1024] = {0};
  char errmsg[2048] = {0};

  void(*errfn)(char *) = NULL;

  switch(error_type) {
  case WARNING:
    strcat(prefixbuffer, "libwebshare(%d) warning: ");
    if(share) errfn = share->config.error_callbacks.warning_cb;
    break;
  case CRITICAL:
    strcat(prefixbuffer, "libwebshare(%d) CRITICAL: ");
    if(share) errfn = share->config.error_callbacks.critical_cb;
    break;
  case FATAL:
    strcat(prefixbuffer, "libwebshare(%d) FATAL: ");
    if(share) errfn = share->config.error_callbacks.fatal_cb;
    break;
  case MESSAGE:
  default:
    strcat(prefixbuffer, "libwebshare(%d): ");
    if(share) errfn = share->config.error_callbacks.message_cb;
    break;
  }

  /* get the process id and fmt it into the buffer */
  sprintf(linebuffer, prefixbuffer, getpid());

  /* append the formatting string passed in */
  strncat(linebuffer, fmt, 1023-strlen(linebuffer));
  
  va_list argp;
  va_start(argp, fmt);

  vsprintf(errmsg, linebuffer, argp);
  
  va_end(argp);
  
  if(share == NULL || errfn == NULL) {
    fprintf(stderr, errmsg);
    if(error_type == FATAL) exit(1);
    return;
  } else {
    errfn(errmsg);
  }

  if(error_type == FATAL) {
    exit(1);
  }

}

void webshare_register_error_callbacks(WEBSHARE *share, int type, void(*errfn)(char *))
{
  if(share == NULL) return;

  switch(type) {
  case WARNING:
    share->config.error_callbacks.warning_cb = errfn;
  case CRITICAL:
    share->config.error_callbacks.critical_cb = errfn;
  case FATAL:
    share->config.error_callbacks.fatal_cb = errfn;
  case MESSAGE:
  default:
    share->config.error_callbacks.message_cb = errfn;
  };

}

#endif
