/***************************************************************************
 *   Copyright (C) 2009 by Adam Risi                                       *
 *   ajrisi@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
/**
 * @file   wsserver.c
 * @author Adam Risi
 * @date   Fri Feb 13 2009
 * 
 * @brief  webshare server.
 * 
 * 
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "hash.h"
#include "reply_codes.h"
#include "default_pages.h"

/* GLOBAL VARIABLES */
static hshtbl *webpage_table;


int wsserver_init() {
  
  return 0;
}


int wsserver_shutdown() {

  return 0;
}

#ifdef WSSERVERTEST

int main(int argc, char **argv) {
  char *statpage = wsserver_status_page(C404, "Example 404");
  printf("%s\n\n", statpage);
  free(statpage);
  return 0;
}

#endif
