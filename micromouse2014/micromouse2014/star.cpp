//
//  star.cpp
//  micromouse2014
//
//

#include "star.h"
#include "cell.h"
#include "packet.h"
#include "wall.h"
#include <math.h>

#define cellsize 16
bool deadend = false;




/********** Star ********/
star::star()
{
	
	direction = "north";
	compass = 90; // starting position is 90 degrees, which will be "north". 0 degrees is to the left to match up with the lidar
	shift = 0.0; // default position, so shift is 0 degrees
	num_packets = 90;
	mazeSize = 16;
	startingRow = 0;
	startingColumn = 0;
	atJunction = false;


	// The goal cell's attributes, assuming the goal is always the center
	goalX = (mazeSize / 2) * cellsize;	// halfway through the maze, multiplied by the centimeters in each cell
	goalY = goalX;

	// size of cells
	lengthwidth = 16; // free to change
	threshold = 2;

	rightTurns = 0;
	leftTurns = 0;

	// create the maze
	createMaze();

	/********* Give the cells their toCost, the estimated cost to the goal ******/


	// scan and update the maze
	while (true)
	{
		scan();	headOnDistance = -1; // set headOnDistance to the packet that represents what is directly in front
		updateMaze();
		goForwardOne();
		scan();	headOnDistance2 = -1;
		updateMaze();
		PositionChange();

		decide(); // decide will check current spot, choose to go forward, left, right, blah blah
	}

	

}

bool closeEnough(double angle1, double angle2)
{
	if (abs(angle2 - angle1) <= 1.0)
		return true;
	else
		return false;
}

// turn by so many degrees, determined by time
// assuming the front middle is 0 degrees
void star::turn(double angle)
{
	double middle; // just here temporarily
	double degrees = middle - angle;
	
	if (degrees > 0) // turn left
	{
		if (ceil(degrees) == 0) //  go straight
		{

		}
		else
		{
			// turn left, degrees per second

			compass += degrees;
		}			
	}
	else if (degrees < 0) // turn right
	{
		if (floor(degrees) == 0) // go straight
		{

		}
		else
		{
			// turn right, degrees per second

			compass += degrees;
		}
	}
}

void polarToCartesian(double radius, double angle, double &x, double &y)
{
	x = radius * cos(angle);
	y = radius * sin(angle);
}

void star::scan()
{
	vision.empty(); // empty the previous vision. new scan
	vect nums; // used to separate the measurements into degrees, 4 numbers per degree
	for (int i = 0; i < 1440; i++) // 1440 measurements total
	{
		// read in 
		double fake;
		double radius;

		if ((i > 180) && (i < 540)) // between angle 45 and 135. might need to expand in order to scan more at once & create the maze faster
		{
			nums.push_back(fake);
			if (nums.size() == 4)
			{
				packet *pack = new packet(nums[0], nums[1], nums[2], nums[3], i); 
				nums.empty();
				vision.push_back(pack);
			}
		}
	}
}

// create a maze with a coordinates that start in the bottom left
void star::createMaze()
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

void star::addCell(cell &newcell) // add in reverse...
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
cell *star::findCell(double x, double y)
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


/*********** movement **********/

// go forward in a small enough chunk (one cell approx.) so that the distance to the opposing wall can be scanned/updated
void star::goForwardOne()
{

}

//called after movement
void star::PositionChange()
{
	double distance = abs(headOnDistance - headOnDistance2);

	if ((compass > 315) && (compass < 45))
	{ // facing left
		xDistance -= distance;
	}
	else if ((compass > 45) && (compass < 135))
	{ // default direction
		yDistance += distance;
	}
	else if ((compass > 135) && (compass < 225))
	{ // facing right
		xDistance += distance;
	}
	else if ((compass > 225) && (compass < 315))
	{ // facing south
		yDistance -= distance;
	}
	else if ((compass == 45) || (compass == 135) || (compass == 225) || (compass == 315))
	{
		// hmmmm......

		// possibly a turn instruction
	}
}

// take the scan, add walls to the existing map of the maze, and call the decision-making function
// the return value means
/*********should turn to see more if the end of a wall is within the last packet*********/
int star::updateMaze()
{
	// add walls to wall deque. probably should turn in the direction of the longer wall to record the pathway in full

	// value that says the packets' radii are too far apart, indicating a space
	double closeEnough = 1;

	std::vector<packet*>::iterator pI; // iterate through vector of packets
	pI = vision.begin();
	std::vector<packet*>::iterator beginner; // is the beginning of the wall
	beginner = vision.begin();
	std::vector<packet*>::iterator follower;
	follower = vision.begin();

	int i = 0;

	string wallOrientation;
	double wallDisplacement_x, wallDisplacement_y, distanceToWall;
	double previous_value;
	pI++;
	// create walls based on the scan
	while (pI != vision.end())
	{
		// create a wall if the difference between the two distances is too large.
		if (((*pI)->aveRadius - (*follower)->aveRadius) > closeEnough)
		{
			
			int angle = (*follower)->angle - (*beginner)->angle; //angle that encompasses the wall from the viewpoint
			if (angle < 0)
				angle = -1 * angle;
			// length from beginning spot to the last spot that was recorded as part of the same wall
			double length;
			// create the wall, orient it, and add it to the maze
			wall *nWall = new wall((*beginner)->aveRadius, (*follower)->aveRadius, (*beginner)->angle, (*follower)->angle);
			wallOrienter(*nWall, wallOrientation, wallDisplacement_x, wallDisplacement_y, distanceToWall);
			addBasedOnCompass(*nWall, wallOrientation, wallDisplacement_x, wallDisplacement_y, distanceToWall);

			pI++; // the ahead iterator
			follower++;
			beginner = follower; // set the beginner to the new wall
		}
		else // just increment the two iterators, not the beginner
		{
			pI++;
			follower++;
		}
	}



	return 0;
}

// get the wall's orientation relative to the robot's field of vision
// called by updateMaze
void star::wallOrienter(wall &wallInQuestion, string &orientation, double &x_displacement, double &y_displacement, double distanceToWall)
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
void star::addBasedOnCompass(wall &wallInQuestion, string wallOrientation, double &x_displacement, double &y_displacement, double distanceToWall)
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

// mark the walls of the cells along which the wall runs.
// coordinateAlongWall: the coordinate along the wall is the one on which all the test points are based
// xORy: x means x corresponds to the coordinateAlongWall. y means y corresponds to the coordinateAlongWall
void star::markWalls(wall &wallInQuestion, double &staticCoord, double &coordinateAlongWall, string &xORy) 
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

// decision-making 
int star::decide()
{
	// get the current cell in which the robot is located
	cell * currentCell = findCell(xDistance, yDistance);
	
	if (atJunction == true)
	{





	}
	
	return 0;
}

void star::determineFromCost(cell &ce)
{
	cell * cellIt;
	cellIt = findCell(0, 0);



}

void star::determineToCost()
{
	// it's okay to measure from the exact center rather than the range of the cell in the center
	goalX = mazeSize * cellsize * 0.5;
	goalY = goalX;


}