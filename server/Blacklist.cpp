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
 *		 Jamie Curtis
 *		 Jesse Pouw-Waas
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

#include <errno.h>
#include "Blacklist.h"
#include "debug.h"

blacklist::blacklist(const char *path)
{
	BLpath = path;
	BLpath += "blacklist.txt";
	load();
}

void blacklist::load(void)
{
	FILE *f=fopen(BLpath.c_str(),"r");
	if (!f) {
		Log(LOG_DAEMON|LOG_WARNING,"Can't read blacklist file %s\n",BLpath.c_str());
		return;
	}
	while(!feof(f)) {
		char buf[80];
		in_addr_t addr;
		if (fgets(buf,sizeof(buf),f) == NULL) {
			if (ferror(f)) {
				Log(LOG_DAEMON | LOG_WARNING, "Error reading from blacklist file: %s\n", strerror(errno));
			}
			fclose(f);
			return;
		}
		if (strchr(buf,'\n')) {
			*strchr(buf,'\n')='\0';
		}
		addr = inet_addr(buf);
		set_light(addr);
	}
	fclose(f);
}

bool blacklist::is_dark(uint32_t ip)
{
	return( lightlist.find(ip) == lightlist.end() );
}

void blacklist::set_light(uint32_t ip)
{
	lightlist.insert(ip);
}

void blacklist::save(void)
{
	FILE *f=fopen(BLpath.c_str(),"w");

	if( f == NULL )
	{
		Log(LOG_DAEMON|LOG_WARNING, 
				"Warning: Failed to write out lightlist: %s\n",
				strerror(errno) );
		return;
	}

	for(lightlist_t::const_iterator i=lightlist.begin();
			lightlist.end() != i;
			++i) {
		fprintf(f,"%s\n",inet_ntoa(*(struct in_addr*)&*i));
	}
	fclose(f);
}

blacklist::~blacklist(void)
{
	save();
}
