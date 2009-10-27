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
 * @file   webshare_types.h
 * @author Adam Risi
 * @date   Tue Feb 10 2009
 * 
 * @brief  Short Description Here
 * 
 * 
 */

#ifndef _WEBSHARE_TYPES_H_
#define _WEBSHARE_TYPES_H_

#include <webshare.h>

typedef struct webshare_s WEBSHARE;

struct webshare_s {
  webshare_config config;
};

typedef struct webshare_config_s webshare_config;

struct webshare_config_s {
  char *host;
  short port;
};

typedef struct webshare_data_s webshare_data;

struct webshare_data_s {
	//this will contain the remote info
};

#endif
