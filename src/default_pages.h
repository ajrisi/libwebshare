/**
 * @file   default_pages.h
 * @author adam <adam@chronos.rh.rit.edu>
 * @date   Sun Feb 15 19:13:31 2009
 * 
 * @brief  Contains the biffers for the default error pages.
 * 
 * 
 */

#ifndef _DEFAULT_PAGES_H_
#define _DEFAULT_PAGES_H_

#include "reply_codes.h"

extern char *status_page_fmt;

extern char *status_code_strs[];

extern char *status_code_detailed_strs[];

/** 
 * Generates an HTML page on the heap that can be served. It uses a
 * very simple default format
 * 
 * @param status_code code in the form CXXX, ex. C404
 * @param details the details of why the error occurred
 * 
 * @return the page bytes
 */
char *wsserver_status_page(int status_code, char *details);

#endif
