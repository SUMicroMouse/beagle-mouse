//
//  cell.h
//  micromouse2014
//
//  Created by Lyle Moffitt on 1/21/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#ifndef __micromouse2014__node__
#define __micromouse2014__node__

#include <iostream>

#include "config.h"

enum direction
{
	NORTH,
	SOUTH,
	EAST,
	WEST
};

class cell
{
	cell *	adj_north;
	cell *	adj_south;
	cell *	adj_east;
	cell *	adj_west;
	
	bool	wall_north;
	bool	wall_south;
	bool	wall_east;
	bool	wall_west;

	bool start_node; // for cells that have multiple options for different paths
	bool state; // used to determine if the path is open or closed (already tried or not)
	
public:
	cell();
	cell(cell & _adj, direction _dir);
	
	void set_adjacent(cell & _adj);
	
};


#endif /* defined(__micromouse2014__node__) */
