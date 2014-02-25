//
//  grid.h
//  micromouse2014
//
// Created by Carlton Semple, Lyle Moffitt, & Nick Beni
// Created at Syracuse University for the 2014 micromouse competition
//

#ifndef __micromouse2014__grid__
#define __micromouse2014__grid__


#include <iostream>
#include <vector>



#include "config.h"
#include "cell.h"




class grid
{
	friend class star;
	
    // A pair of axis intercepts for each cell element
    // The intercept is a line of cell references
	// It should not be assumed that these are cartesian coordinates
	std::vector<std::vector<cell*>>	axis_x;
	std::vector<std::vector<cell*>>	axis_y;
	
	cell	*	origin;
	cell	*	goal;
	cell	*	center;
	
public:
	grid();
	grid(uint x_dim, uint y_dim);
	
};

#endif /* defined(__micromouse2014__grid__) *//
