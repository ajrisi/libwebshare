/* main header file for the webshare server */
#ifndef _WSSERVER_H_
#define _WSSERVER_H_

#include "hash.h"

/* CONFIGURABLE VALUES */
#define WSSERVER_MAX_HEADER_LENGTH 1024


/* VARIABLES */
extern hshtbl *webpage_table;


/* FUNCTIONS */

int wsserver_init( void );

int wsserver_shutdown( void );

#endif
