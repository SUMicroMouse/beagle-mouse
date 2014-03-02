//
//  cell.h
//  micromouse2014
//
// Created by Carlton Semple, Lyle Moffitt, & Nick Beni
// Created at Syracuse University for the 2014 micromouse competition
//

#ifndef __micromouse2014__cell__
#define __micromouse2014__cell__

#include <iostream>


#include "config.h"

typedef
std::string string;

//enum direction
//{
//	NORTH,
//	SOUTH,
//	EAST,
//	WEST
//};

/* A cell is just an arbitrary set of 4 "walls" that enclose a space. Not all of
 the walls are physically there. The walls should not be assumed to be parallel
 or of a fixed size.

 The graph below is exerpted from <lidar.h>. It contains many cells. The number,
 size, and shape of each depends on your view. From a top-down, cartesian 
 perspective, it could be said that there are 9 othogonal, equilateral, cells.
 From the @'s co-planar perspective there are between 4 and 6  cells (approx).
 They are radial or wedge shaped, with varying size and shape. 
 
 +       + ----- + ----- +
 |               |
 |               |
 |               |
 + ----- +       + ----- +
 |       |
 |       |
 |       |
 +       +       + ----- +
 |               |       |
 |           @   |       |
 |               |       |
 +               +       +
 
 A cell has adjacentcies. Not all of them are filled. They exist if any two
 cells share a "wall". 
 */
class cell
{
public:
	int row; // lat
	int column; // long

	cell *	north;
	cell *	south;
	cell *	east;
	cell *	west;

private:
	bool	b_north; // b for boundary
	bool	b_south;
	bool	b_east;
	bool	b_west;

	bool start_node; // for cells that have multiple options for different paths
	bool state; // used to determine if the path is open or closed (already tried or not)
	
public:
	cell();
	cell(int r, int c);
	cell(int left, int right, int front, string direction);
	//cell(cell & _adj, direction _dir);
	
	void set_adjacent(cell & _adj);

	void sNorth(bool g) { b_north = g; }
	void sSouth(bool g) { b_south = g; }
	void sEast(bool g) { b_east = g; }
	void sWest(bool g) { b_west = g; }

	bool gNorth() { return b_north; }
	bool gSouth() { return b_south; }
	bool gEast() { return b_east; }
	bool gWest() { return b_west; }

	void markSourceDirection(string direction);
	void checkVirtualSides(bool &left, bool &right, bool &front, string direction);
	
	int checked; // 1 = checked. 2 = has multiple paths...
	bool deadend;
	bool finish;

};


#endif /* defined(__micromouse2014__cell__) */
