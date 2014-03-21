//
//  cell.cpp
//  micromouse2014
//
//  Created by Lyle Moffitt on 1/21/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#include "cell.h"

using namespace std;

#define cellsize 16.0
#define mazesize 16.0

cell::cell()
{
	checked = 0;
    east = north = south = west = nullptr;
	deadend = false;
	finish = false;
}

cell::cell(int r, int c)
{
	row = r;
	column = c;

	checked = 0;
	deadend = false;
	finish = false;
	b_north = 0;
	b_south = 0;
	b_east = 0;
	b_west = 0;

	goalCell = false;

	x_center = (cellsize * column) + 0.5 * cellsize;
	y_center = (cellsize * row) + 0.5 * cellsize;
}

// take the coordinates in centimeters and create a cell with the correct row/column attributes
cell::cell(double x, double y)
{
	double rtemp = y / cellsize;
	double ctemp = x / cellsize;

	row = floor(rtemp);
	column = floor(ctemp);
}

// uses the current direction to determine which walls are false or true
cell::cell(int left, int right, int front, string direction)
{
	deadend = false;
	finish = false;
	if (direction == "north")
	{
		b_south = true; // set to true so that when returning to the cell, it doesn't go down a path previously traveled

		if (left == 1)
			b_west = true;
		if (right == 1)
			b_east = true;
		if (front == 1)
			b_north = true;
	}
	else if (direction == "south")
	{
		b_north = true;

		if (left == 1)
			b_east = true;
		if (right == 1)
			b_west = true;
		if (front == 1)
			b_south = true;
	}
	else if (direction == "east")
	{
		b_west = true;

		if (left == 1)
			b_north = true;
		if (right == 1)
			b_south = true;
		if (front == 1)
			b_east = true;
	}
	else if (direction == "west")
	{
		b_east = true;

		if (left == 1)
			b_south = true;
		if (right == 1)
			b_north = true;
		if (front == 1)
			b_west = true;
	}
}

// upon revisiting a cell, "close" the direction that you're coming from
void cell::markSourceDirection(string direction)
{
	if (direction == "north")
		b_south = true; // set to true so that when returning to the cell, it doesn't go down a path previously traveled
	else if (direction == "south")
		b_north = true;
	else if (direction == "east")
		b_west = true;
	else if (direction == "west")
		b_east = true;
}

// returns the values of left, front, right depending on the direction
void cell::checkVirtualSides(int &left, int &right, int &front, string direction)
{
	if (direction == "north")
	{
		left = b_west;
		right = b_east;
		front = b_north;
	}
	else if (direction == "south")
	{
		left = b_east;
		right = b_west;
		front = b_south;
	}
	else if (direction == "east")
	{
		left = b_north;
		right = b_south;
		front = b_east;
	}
	else if (direction == "west")
	{
		left = b_south;
		right = b_north;
		front = b_west;
	}
}

// returns true if the point is closer to sideA than sideB
bool closerTo(double sideA, double sideB, double point)
{
	double d1, d2;

	d1 = abs(sideA - point);
	d2 = abs(sideB - point);

	if (d1 < d2)
		return true;
	else
		return false;
}

// mark the wall and go to the neighboring cell and mark its corresponding wall
// mode: 1 = mark. -1 = unmark/empty
// 1 = north. 2 = south. 3 = west. 4 = east.
void cell::wallMark(int side, int mode)
{
	cell *otherCell;

	if (mode == 1) // mark the wall as being present
	{
		switch (side)
		{
		case 1:{
				   b_north = 1;
				   otherCell = north;
				   otherCell->b_south = 1;
		}
			break;
		case 2:{
				   b_south = 1;
				   otherCell = south;
				   otherCell->b_north = 1;
		}
			break;
		case 3:{
				   b_west = 1;
				   otherCell = west;
				   otherCell->b_east = 1;
		}
			break;
		case 4:{
				   b_east = 1;
				   otherCell = east;
				   otherCell->b_west = 1;
		}
			break;
		default:
			break;
		}
	}
	else if (mode == -1) // make the wall nonexistent
	{
		switch (side)
		{
		case 1:{
				   b_north = 0;
				   otherCell = north;
				   otherCell->b_south = 0;
		}
			break;
		case 2:{
				   b_south = 0;
				   otherCell = south;
				   otherCell->b_north = 0;
		}
			break;
		case 3:{
				   b_west = 0;
				   otherCell = west;
				   otherCell->b_east = 0;
		}
			break;
		case 4:{
				   b_east = 0;
				   otherCell = east;
				   otherCell->b_west = 0;
		}
			break;
		default:
			break;
		}
	}
	
}

// check to see if the point is within the cell, including the walls
void cell::markWalls(double x, double y, double sourceX, double sourceY)
{
	double rightBound = x_center + 0.5 * cellsize;
	double leftBound = x_center - 0.5 * cellsize;
	// remember that the top left is the origin of the grid
	double topBound = y_center + 0.5 * cellsize;
	double bottomBound = y_center - 0.5 * cellsize;

	cell * otherCell;

	// check to see if the point is within the four walls
	if ((x >= leftBound) && (x <= rightBound))
	{
		if ((y >= bottomBound) && (y <= topBound))
		{
			if (closerTo(leftBound, rightBound, x))
			{	// closer to left wall
				if (closerTo(topBound, bottomBound, y))
				{	// closer to top wall
					if (abs(leftBound - x) < abs(topBound - y))
					{	// left wall must be present rather than the top wall
						// turn the left boundary to true, and go to that cell and change its right boundary to true
						wallMark(3, 1);
					}
					else
					{	// top wall must be present instead of the left all
						wallMark(1, 1);
					}
				}
				else
				{	// closer to bottom wall
					if (abs(leftBound - x) < abs(bottomBound - y))
					{	// left wall is present
						wallMark(3, 1);
					}
					else
					{	// bottom wall is present
						wallMark(2, 1);
					}
				}				
			}
			else
			{	// closer to right wall
				if (closerTo(topBound, bottomBound, y))
				{	// closer to top wall
					if (abs(rightBound - x) < abs(topBound - y))
					{	// right wall must be present instead of the top wall
						wallMark(4, 1);
					}
					else
					{	// top wall must be present instead of the right wall
						wallMark(1, 1);
					}
				}
				else
				{	// closer to bottom wall
					if (abs(rightBound - x) < abs(bottomBound - y))
					{	// right wall must be present instead of the bottom wall
						wallMark(4, 1);
					}
					else
					{	// bottom wall must be present instead of the right wall
						wallMark(2, 1);
					}
				}	
			}
		}
	}


	/* Naturally, when looking down an empty space, it can be assumed that for each of the cells, the wall closest to the robot is open; otherwise, it would not be possible to see through the cell
	 mark the sides closest to the robot as open only if it's unknown, to avoid error in the situations where the wall is partially seen, but not fully, blah blah. it works out in the end, with the earlier part of this function.
	 (an open wall can be closed if it's determined that a wall is there, but not the other way around) */
	declareSideEmpty(sourceX, sourceY);
}

//
void cell::declareSideEmpty(double sourceX, double sourceY)
{
	double rightBound = x_center + 0.5 * cellsize;
	double leftBound = x_center - 0.5 * cellsize;
	// remember that the top left is the origin of the grid
	double topBound = y_center + 0.5 * cellsize;
	double bottomBound = y_center - 0.5 * cellsize;




	if (closerTo(leftBound, rightBound, sourceX))
	{	// closer to left wall
		if (closerTo(topBound, bottomBound, sourceY))
		{	// closer to top wall
			if (abs(leftBound - sourceX) < abs(topBound - sourceY))
				wallMark(3, -1);
			else	
				wallMark(1, -1);
		}
		else
		{	// closer to bottom wall
			if (abs(leftBound - sourceX) < abs(bottomBound - sourceY))
				wallMark(3, -1);
			else
				wallMark(2, -1);
		}
	}
	else
	{	// closer to right wall
		if (closerTo(topBound, bottomBound, y))
		{	// closer to top wall
			if (abs(rightBound - sourceX) < abs(topBound - sourceY))	
				wallMark(4, -1);
			else
				wallMark(1, -1);
		}
		else
		{	// closer to bottom wall
			if (abs(rightBound - sourceX) < abs(bottomBound - sourceY))
				wallMark(4, 1);
			else
				wallMark(2, -1);
		}
	}

}

// figure out the heuristicCost, which is simply based on the distance to the Goal.
void cell::figureheuristicCost(double goalX, double goalY)
{
	double distance;

	distance = sqrt(((x_center - goalX) * (x_center - goalX)) + ((y_center - goalY) * (y_center - goalY)));

	heuristicCost = distance; // centimeters give a high number...
}

// return true if any sides other than the source side are open
bool cell::declareSidesOpen(char sourceSide)
{
	/********** IF TWO ADDITIONAL SIDES ARE OPEN, NOT JUST ONE **********/
	//
		//
		//
	int count = 0;

	switch (sourceSide)
	{
	case 'n':{	// north
				 if (b_east == -1)
					 count++;
				 if (b_south == -1)
					 count++;
				 if (b_east == -1)
					 count++;
	}
		break;
	case 's':{	// south
				 if (b_east == -1)
					 count++;
				 if (b_north == -1)
					 count++;
				 if (b_west == -1)
					 count++;
	}
		break;
	case 'e':{	// east
				 if (b_north == -1)
					 count++;
				 if (b_south == -1)
					 count++;
				 if (b_west == -1)
					 count++;
	}
		break;
	case 'w':{	// west
				 if (b_east == -1)
					 count++;
				 if (b_south == -1)
					 count++;
				 if (b_north == -1)
					 count++;
	}
		break;
	default:
		break;
	}

	if (count > 1)
		return true;
	else
		return false;
}

void cell::returnSides(int &north, int &south, int &east, int &west)
{
	north = b_north;
	south = b_south;
	east = b_east;
	west = b_west;
}
// return values of 1 for closed sides and -1 for open sides
void cell::returnSides(int &north, int &south, int &east, int &west, char &sourceDirection)
{
	switch (sourceDirection)
	{
	case 'n':{
				 south = b_south;
				 east = b_east;
				 west = b_west;
				 // close off previous side
				 north = 1;
	}
		break;
	case 's':{
				 north = b_north;
				 east = b_east;
				 west = b_west;
				 // close off previous side
				 south = 1;
	}
		break;
	case 'e':{
				 north = b_north;
				 west = b_west;
				 south = b_south;
				 // close off previous side
				 east = 1;
	}
		break;
	case 'w':{
				 north = b_north;
				 east = b_east;
				 south = b_south;
				 // close off previous side
				 west = 1;
	}
		break;

	default:
		break;
	}
}