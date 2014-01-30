//
//  star.h
//  micromouse2014
//
//

#ifndef __micromouse2014__star__
#define __micromouse2014__star__

#include <iostream>
#include <vector>
#include <stack>


#include "grid.h"
#include "config.h"

typedef
std::stack<grid &> path;

// contains 
class item{

};

/* This will simplify data input into 19 regions. 
The angle of the data from the lidar will determine the position in viewfinder, 
	and the distance/radius will be used to determine when an open space is coming up on the left/right, or a wall ahead.
The number of items in the viewfinder may be increased for better accuracy and increased response time*/
class local_grid{
	std::vector<double> viewfinder;
	std::vector<double> previous;
public:
	local_grid();

	// takes the grid from the lidar scan and simplifies the data
	void updateView(grid &fetched_scan, int &returnedLeft, int &returnedRight, double &returnedFront); 
};


class star
{
	// size of cells
	double lengthwidth;
	double threshold; // distance at which it's determined the side walls aren't there

	grid maze;
	
	path to_orig;
	path guess;

	/*********** Local Grid ************/
	int leftFromViewfinder; // will get the region that the open space (to the left) is currently at in relation to the lidar
	int rightFromViewfinder; // will get the region that the open space (to the right) is currently at in relation to the lidar
	double frontFromViewfinder; // gets the distance of the region directly in front
public:
	star();

	grid *get_scan(); // retrieves scan from lidar 
	void motion(grid &fetched_scan); // overall motion. calls the turn functions
	
	local_grid viewFinder;
};

#endif /* defined(__micromouse2014__star__) */
