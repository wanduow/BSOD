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


#ifndef BLACKLIST_H
#define BLACKLIST_H

#include "string"
#include <string.h>
#include "stdio.h"
#include "stdlib.h"
#include "libtrace.h"
#include "time.h"
#include <arpa/inet.h>
#include <set>

class blacklist
{
public:
	blacklist(const char *path);

	bool is_dark(uint32_t ip);
	void set_light(uint32_t ip);

	void save(void);

	~blacklist();

private:
	typedef std::set<in_addr_t> lightlist_t;
	lightlist_t lightlist;

	std::string BLpath;
	void load(void);
};

#endif
