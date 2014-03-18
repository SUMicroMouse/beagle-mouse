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
	// 0 = unknown. -1 = onfirmed, no wall. 1 = wall confirmed present
	int	b_north; // b for boundary
	int	b_south;
	int	b_east;
	int	b_west;

	bool start_node; // for cells that have multiple options for different paths
	bool state; // used to determine if the path is open or closed (already tried or not)

	double x_center, y_center;
	
public:
	cell();
	cell(int r, int c);
	cell(double x, double y);
	cell(int left, int right, int front, string direction);
	//cell(cell & _adj, direction _dir);
	
	void set_adjacent(cell & _adj);

	void sNorth(bool g) { b_north = g; }
	void sSouth(bool g) { b_south = g; }
	void sEast(bool g) { b_east = g; }
	void sWest(bool g) { b_west = g; }

	int gNorth() { return b_north; }
	int gSouth() { return b_south; }
	int gEast() { return b_east; }
	int gWest() { return b_west; }

	void markSourceDirection(string direction);
	void checkVirtualSides(int &left, int &right, int &front, string direction);
	void markWalls(double x, double y, double sourceX, double sourceY);
	void wallMark(int side, int mode);
	void declareSideEmpty(double sourceX, double sourceY);

	int checked; // 1 = checked. 2 = has multiple paths...
	bool deadend;
	bool finish;

	double fromCost; // cost from the origin
	double toCost; // estimated cost to the goal

	void figureToCost(double goalX, double goalY);
};


#endif /* defined(__micromouse2014__cell__) */
