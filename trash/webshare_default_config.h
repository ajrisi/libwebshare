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
 * @file   webshare_default_config.h
 * @author Adam Risi
 * @date   Fri Feb 13 2009
 * 
 * @brief  This file contains the #defines for all of the default webshare settings.
 * 
 * 
 */

/*
 *Server component settings
 */

#ifndef WSD_SERVER_PORT
	#define WSD_SERVER_PORT 8081
#endif

/*
 * Client component settings
 */

#ifndef WSD_MAX_CLIENTS
	#define WSD_MAX_CLIENTS 10
#endif

/*
 * General settings 
 */

#ifndef WSD_MAX_THREADS
	#define WSD_MAX_THREADS 15
#endif
