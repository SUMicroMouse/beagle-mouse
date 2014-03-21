//
//  grid.cpp
//  micromouse2014
//
//  Created by Lyle Moffitt on 1/21/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#include "grid.h"
#include "wall.h"

using namespace std;

grid::grid():
lat_headers(*new vector<vector<cell*>>(1,vector<cell*>(1, nullptr))),
long_headers(*new vector<vector<cell*>>(1,vector<cell*>(1, nullptr)))
{
    origin = goal = center = nullptr;
}

// find the current cell based on the row/column
cell * 
grid::getCell(double row, double column)
{
	cell *point;
	
	int r = row;
	int c = column;
    
	for (int i = 0; i < mazeSize; i++)
	{
		point = lat_headers[i];
		while (point != nullptr)
		{
			if ((point->column == c) && (point->row == r))
				return point;
            
			point = point->east;
		}
	}
}

// create a maze with a coordinates that start in the bottom left
void grid::createMaze()
{
	lat_headers.resize(mazeSize);
	long_headers.resize(mazeSize);
    
	int r, c;
	for (r = 0; r < mazeSize; r++) // create in reverse so that the origin of the coordinates is in the bottom left
	{
		for (c = 0; c < mazeSize; c++)
		{
			cell *temp = new cell(r, c);
			addCell(*temp);
		}
	}
}

void grid::markGoalCells()
{
	double row1, row2, column1, column2;
	row1 = floor(mazeSize / 2);
	row2 = row1 + 1;
	column1 = floor(mazeSize / 2);
	column2 = column1 + 1;
    
	cell *cellP = getCell(row1, column1);
    cellP->goalCell = true;
	cellP = getCell(row1, column2);
    cellP->goalCell = true;
	cellP = getCell(row2, column1);
    cellP->goalCell = true;
	cellP = getCell(row2, column2);
    
    
}

void grid::addCell(cell &newcell) // add in reverse...
{
	cell *adder;
	adder = lat_headers[mazeSize - (newcell.row + 1)]; // reverse
	if (adder != NULL)
	{
		while (adder->east != NULL)
		{
			adder = adder->east;
			if (adder->east == NULL)
				break;
		}
		if (adder->east == NULL)
		{
			newcell.west = adder;
			adder->east = &newcell;
		}
	}
	else
		lat_headers[mazeSize - (newcell.row + 1)] = &newcell;
    
	// link it to the column
	cell *linker;
	linker = long_headers[newcell.column];
	if (linker != NULL)
	{
		while (linker->south != NULL)
		{
			linker = linker->south;
			if (linker->south == NULL)
				break;
		}
		if (linker->south == NULL)
		{
			newcell.north = linker;
			linker->south = &newcell;
		}
	}
	else
		long_headers[newcell.column] = &newcell;
}

// find the current cell based on the coordinates in centimeters
cell *grid::findCell(double x, double y)
{
	double rtemp = y / cellsize;
	double ctemp = x / cellsize;
    
	int row = floor(rtemp);
	int column = floor(ctemp);
    
	cell *point;
    
	for (int i = 0; i < mazeSize; i++)
	{
		point = lat_headers[i];
		while (point != nullptr)
		{
			if ((point->column == column) && (point->row == row))
				return point;
            
			point = point->east;
		}
	}
}


// get the wall's orientation relative to the robot's field of vision
// called by updateMaze
void grid::wallOrienter(wall &wallInQuestion, string &orientation, double &x_displacement, double &y_displacement, double distanceToWall)
{
	// only five possibilities
	double angle;
    
	if (closeEnough(wallInQuestion.leftAngle, wallInQuestion.rightAngle))
	{	// wall is directly ahead
        
		// get triangle height to see how far away it is
        
		//triangle perimeter
		double perimeter = wallInQuestion.radiusLeft + wallInQuestion.length + wallInQuestion.radiusRight;
        
		// Heron's formula for area
		double area = sqrt(perimeter * (perimeter - wallInQuestion.radiusLeft) * (perimeter - wallInQuestion.length) * (perimeter - wallInQuestion.radiusRight));
        
		// area = (1/2) * base * height
		// height = (2*area)/base . base will be wallinquestion.length. distanceToWall will be height
		distanceToWall = (2 * area) / wallInQuestion.length;
		x_displacement = 0;
		y_displacement = 0;
	}
	else if (wallInQuestion.leftAngle < wallInQuestion.rightAngle)
	{
		// assuming 90 degrees is directly in the middle
		if (wallInQuestion.a1 > 90.0)
		{
			// the wall is perpendicular to the robot's plane of vision (parallel with the 90 degree line)
			orientation = "perpendicularRight";
			
			angle = 180 - wallInQuestion.a2;
			x_displacement = cos(angle) * wallInQuestion.radiusRight; // tricky here
			y_displacement = sin(angle) * wallInQuestion.radiusRight;
		}
		else
		{
			// the wall is parallel to the robot's plane of vision (parallel with the 0 degree line)
			orientation = "parallelLeft";
            
			angle = wallInQuestion.a2; // rightVectorAngle.
			x_displacement = -cos(angle) * wallInQuestion.radiusRight; // x_displacement left is negative. right is positive
			y_displacement = sin(angle) * wallInQuestion.radiusRight;
		}
	}
	else if (wallInQuestion.rightAngle < wallInQuestion.leftAngle)
	{
		// assuming 90 degrees is directly in the middle
		if (wallInQuestion.a2 < 90.0)
		{
			// the wall is perpendicular to the robot's plane of vision
			orientation = "perpendicularLeft";
            
			angle = wallInQuestion.a1;
			x_displacement = -cos(angle) * wallInQuestion.radiusLeft; // x_displacement left is negative. right is positive
			y_displacement = sin(angle) * wallInQuestion.radiusLeft;
		}
		else
		{
			// the wall is parallel to the robot's plane of vision
			orientation = "parallelRight";
            
			angle = 180 - wallInQuestion.a1;
			x_displacement = cos(angle) * wallInQuestion.radiusLeft;
			y_displacement = sin(angle) * wallInQuestion.radiusLeft;
		}
	}
    
}

// take the info of the wall from wallOrienter and add it to the map of the maze
// (calls markWalls)
void grid::addBasedOnCompass(wall &wallInQuestion, string wallOrientation, double &x_displacement, double &y_displacement, double distanceToWall)
{
	double x_wall, y_wall;
	string xx = "x";
	string yy = "y";
    
	// assuming 90 is the starting direction. 0 is to the left at the beginning
	if ((compass > 315) && (compass < 45))
	{ // facing left
      // these are currently the starting coordinates for the wall
		x_wall = xDistance - y_displacement; // positive y displacement will be negative x
		y_wall = yDistance + x_displacement; // positive x is to the right, which will end up being positive y
        
		if ((wallOrientation == "perpendicularRight") || (wallOrientation == "perpendicularLeft")){
			// subtract half the wall's length from the position of the robot to get to the center of the wall
			x_wall -= wallInQuestion.length / 2; // dividing by 2 gets the middle of the wall, which will lead to accurately marking which wall it is
			markWalls(wallInQuestion, y_wall, x_wall, xx);
		}
		else if (wallOrientation == "parallelLeft"){
			y_wall -= wallInQuestion.length / 2; // left is negative y
			markWalls(wallInQuestion, x_wall, y_wall, yy);
		}
		else if (wallOrientation == "parallelRight"){
			y_wall += wallInQuestion.length / 2;
			markWalls(wallInQuestion, x_wall, y_wall, yy);
		}
	}
	else if ((compass > 45) && (compass < 135)) 
	{ // default direction
		x_wall = xDistance + x_displacement;
		y_wall = yDistance + y_displacement;
        
		if ((wallOrientation == "perpendicularRight") || (wallOrientation == "perpendicularLeft")){
			y_wall += wallInQuestion.length / 2; // now the coordinates point to the wall's midpoint
			markWalls(wallInQuestion, x_wall, y_wall, yy);
		}
		else if (wallOrientation == "parallelLeft"){
			x_wall -= wallInQuestion.length / 2;
			markWalls(wallInQuestion, y_wall, x_wall, xx);
		}
		else if (wallOrientation == "parallelRight"){
			x_wall += wallInQuestion.length / 2;
			markWalls(wallInQuestion, y_wall, x_wall, xx);
		}
	}
	else if ((compass > 135) && (compass < 225))
	{ // facing right
		x_wall = xDistance + y_displacement; // positive y will equal positve x
		y_wall = yDistance - x_displacement; // positive x is to the right, which will equal negative y
        
		if ((wallOrientation == "perpendicularRight") || (wallOrientation == "perpendicularLeft")){
			x_wall += wallInQuestion.length / 2;
			markWalls(wallInQuestion, y_wall, x_wall, xx);
		}
		else if (wallOrientation == "parallelLeft"){
			y_wall += wallInQuestion.length / 2;
			markWalls(wallInQuestion, x_wall, y_wall, yy);
		}
		else if (wallOrientation == "parallelRight"){
			y_wall -= wallInQuestion.length / 2; // right is negative y
			markWalls(wallInQuestion, x_wall, y_wall, yy);
		}
	}
	else if ((compass > 225) && (compass < 315))
	{ // facing south
		x_wall = xDistance - x_displacement; // reversed
		y_wall = yDistance - y_displacement; // reversed
        
		if ((wallOrientation == "perpendicularRight") || (wallOrientation == "perpendicularLeft")){
			y_wall -= wallInQuestion.length / 2;
			markWalls(wallInQuestion, x_wall, y_wall, yy);
		}
		else if (wallOrientation == "parallelLeft"){
			x_wall += wallInQuestion.length / 2; // left is positive x
			markWalls(wallInQuestion, y_wall, x_wall, xx);
		}
		else if (wallOrientation == "parallelRight"){
			x_wall -= wallInQuestion.length / 2; // right is negative x
			markWalls(wallInQuestion, y_wall, x_wall, xx);
		}
	}
	else if ((compass == 45) || (compass == 135) || (compass == 225) || (compass == 315))
	{
		// hmmmm......
        
		// possibly a turn instruction
	}
    
	
}

// mark the walls of the cells along which the wall runs, and make mark of the empty walls
// coordinateAlongWall: the coordinate along the wall is the one on which all the test points are based
// xORy: x means x corresponds to the coordinateAlongWall. y means y corresponds to the coordinateAlongWall
// - called by addBasedOnCompass
void grid::markWalls(wall &wallInQuestion, double &staticCoord, double &coordinateAlongWall, string &xORy) 
{
	//************* UPDATE **********************
	// The string is actually not needed. When this function is called in "addBasedOnCompass", the x and y coordinates are switched depending on the situation
	// ******* Actually, it is needed due to how the cell->markWalls function is not written to compensate for those changes. xORy is needed
    
	// mark the wall based on the coordinates x_wall & y_wall .
	cell *cellIt;
	for (int r = 0; r < mazeSize; r++)
	{
		cellIt = lat_headers[r];
		while (cellIt != NULL)
		{
			// do multiple markings along the length of the wall
			double end = wallInQuestion.length / 2;
			double marker = coordinateAlongWall - end; // starting at one end of the wall
			while (marker <= coordinateAlongWall + end)
			{
				// marker takes the place of the coordinateAlongWall
				if (xORy == "x")
					cellIt->markWalls(marker, staticCoord, xDistance, yDistance);
				else if (xORy == "y")
					cellIt->markWalls(staticCoord, marker, xDistance, yDistance);
                
				
				marker += 0.5; // .5 centimeters should be specific enough
			}
			// move to the next cell
			cellIt = cellIt->east;
		}
	}
    
    
}

cell * grid::getPointerToJunction(char &sourceDirection)
{
	// get the current cell in which the robot is located
	cell * currentCell = findCell(xDistance, yDistance);
	cell *cellPoint = currentCell;

	//	Check the cells in front for 
	//	where there's an upcoming junction
	//
	if ((compass > 315) && (compass < 45))
	{ // facing west
		do{
			cellPoint = cellPoint->west;
		} while (cellPoint->declareSidesOpen('e') == false);
		sourceDirection = 'e';
	}
	else if ((compass > 45) && (compass < 135))
	{ // default direction	
		do{
			cellPoint = cellPoint->north;
		} while (cellPoint->declareSidesOpen('s') == false);
		sourceDirection = 's';
	}
	else if ((compass > 135) && (compass < 225))
	{ // facing east
		do{
			cellPoint = cellPoint->east;
		} while (cellPoint->declareSidesOpen('w') == false);
		sourceDirection = 'w';
	}
	else if ((compass > 225) && (compass < 315))
	{ // facing south
		do{
			cellPoint = cellPoint->south;
		} while (cellPoint->declareSidesOpen('n') == false);
		sourceDirection = 'n';
	}
	else if ((compass == 45) || (compass == 135) || (compass == 225) || (compass == 315))
	{
		// hmmmm......
		// possibly a turn instruction
	}

	return cellPoint;
}

cell * grid::findClosestGoalCell(double x, double y)
{

}

bool grid::closeEnough(double angle1, double angle2)
{
	if (abs(angle2 - angle1) <= 1.0)
		return true;
	else
		return false;
}

