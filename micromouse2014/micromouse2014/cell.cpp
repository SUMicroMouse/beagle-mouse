//
//  cell.cpp
//  micromouse2014
//
//  Created by Lyle Moffitt on 1/21/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#include "cell.h"

using namespace std;

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
void cell::checkVirtualSides(bool &left, bool &right, bool &front, string direction)
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

