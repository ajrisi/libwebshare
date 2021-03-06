/**
 * @file   wserror.h
 * @author Adam Risi <ajrisi@gmail.com>
 * @date   Mon Mar  2 07:24:39 2009
 * 
 * @brief  Error logging facilities
 * 
 * 
 */

#ifndef _WSERROR_H_
#define _WSERROR_H_

enum {
  MESSAGE,
  WARNING,
  CRITICAL,
  FATAL
};

void wserror(WEBSHARE *share, int error_type, char *fmt, ...);

/** 
 * Register an error callback in share of type type
 * 
 * @param share the share to register the error callback in
 * @param type the type of the error to register
 * @param errfn the function to call instead of printf inside of wserror
 */
void webshare_register_error_callbacks(WEBSHARE *share, int type, void(*errfn)(char *));

#endif
