/*
 * This file is part of bsod-server
 *
 * Copyright (c) 2004-2011 The University of Waikato, Hamilton, New Zealand.
 * Authors: Brendon Jones
 *          Daniel Lawson
 *          Sebastian Dusterwald
 *          Yuwei Wang
 *          Paul Hunkin
 *          Shane Alcock
 *
 * Contributors: Perry Lorier
 *               Jamie Curtis
 *               Jesse Pouw-Waas
 *          
 * All rights reserved.
 *
 * This code has been developed by the University of Waikato WAND 
 * research group. For further information please see http://www.wand.net.nz/
 *
 * bsod-server is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * bsod-server is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with bsod-server; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * $Id$
 *
 */

#include <stdio.h> 
#include <syslog.h> 
#include <stdlib.h> 
#include <stdarg.h> 
#include <assert.h> 
#include <time.h>
#include <string.h>

#include "debug.h"
#include "daemons.h"

void Log(int priority, const char *fmt, ...)
{
        va_list ap;
        char buffer[513];

        assert(daemonised == 0 || daemonised == 1);

        va_start(ap, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, ap);

        if(! daemonised){
		time_t ts=time(NULL);
		char date[32];
		ctime_r(&ts,date);
		date[strlen(date)-1]='\0';
		fprintf(stderr,"%s: %s",date,buffer);
        } else {
                syslog(priority, "%s", buffer);
        }
        va_end(ap);

}

