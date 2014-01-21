//
//  grid.h
//  micromouse2014
//
//  Created by Lyle Moffitt on 1/21/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#ifndef __micromouse2014__cell_map__
#define __micromouse2014__cell_map__

#include <iostream>
#include <vector>

#include "cell.h"
#include "config.h"


class grid
{
	friend class star;
	
	// It should not be assumed that these are cartesian coordinates
	std::vector<cell &>	axis_x;
	std::vector<cell &>	axis_y;
	
	cell	*	origin;
	cell	*	goal;
	cell	*	center;
	
public:
	grid();
	grid(unsigned grid_size);
	
};

#endif /* defined(__micromouse2014__cell_map__) */
