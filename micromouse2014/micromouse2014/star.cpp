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

bool deadend = false;


void local_grid::updateView(grid &fetched_scan, int &returnedLeft, int &returnedRight, double &returnedFront)
{
	// insert math here. depending on angle, assign to appropriate location in vector
	
	// much of the viewFinder ideally should have consistent values, assuming a perfectly straight line down the middle of the cells. 
	// a fluctuation greater than a to-be-determined number will trigger action

	// hopefully there's a more efficient way to check, but for now....
	double check_left, check_right;
	for (int i = 0; i < viewfinder.size(); i++)
	{
		// if the ends report great distances, then it's safe to assume that they're open spaces to the sides
	}

	// simple check for the center/front
	double front_change = viewfinder[9] - previous[9];
	if (front_change < 0)
		returnedFront = front_change; // some wall is coming up
}

/********** Star ********/
star::star()
{
	
	direction = "north";
	compass = 90; // starting position is 90 degrees, which will be "north". 0 degrees is to the left to match up with the lidar
	shift = 0.0; // default position, so shift is 0 degrees
	num_packets = 90;
	mazeSize = 16;

	// size of cells
	lengthwidth = 16; // free to change
	threshold = 2;

	rightTurns = 0;
	leftTurns = 0;

	leftFromViewfinder = 0;
	rightFromViewfinder = 0;
	frontFromViewfinder = 0;
}

// direction that the mouse is facing
// left turn = 1. right = 2.
void star::changeDirection(int turn)
{
	if (direction == "north")
	{
		if (turn == 1) // facing north, and now turning left
			direction = "west";
		else // turning right, new direction is east
			direction = "east";
	}
	else if (direction == "south")
	{
		if (turn == 1)
			direction = "east";
		else
			direction = "west";
	}
	else if (direction == "east")
	{
		if (turn == 1)
			direction = "north";
		else
			direction = "south";
	}
	else if (direction == "west")
	{
		if (turn == 1)
			direction = "south";
		else
			direction = "north";
	}

}

void star::choose(cell &junction)
{
	// remember to call changeDirection**********************

	// default direction is north. so going south would be probably be bad
	
	// use integer for direction instead
	int d = 0;
	if (direction == "north")
		d = 1;
	else if (direction == "south")
		d = 2;
	else if (direction == "east")
		d = 3;
	else if (direction == "west")
		d = 4;

	// priority given to the side that hasn't had 3 turns in a row
	int turnPriorityFactor = 0;
	if (rightTurns >= 2)
		turnPriorityFactor = -1; // left turn preferred
	else if (leftTurns >= 2)
		turnPriorityFactor = 1; // right turn preferred
	

	switch (d)
	{
		case 1:{ // north
				   // straight preferred
				   if (!junction.gNorth()) // true means that the wall exists
				   {
					   // go straight
					   
					   return;
				   }
				   if ((!junction.gWest()) && (!junction.gEast())) // east & west must be open
				   {
					   if (turnPriorityFactor == -1)
					   { // turn left
						   changeDirection(1);
					   }
					   else
					   {
						   // turn right
						   changeDirection(2);
					   }
				   }

		} break;
		case 2:{ // south
				   // straight not preferred
				   if ((!junction.gWest()) || (!junction.gEast())) // if either right or left is open
				   {
					   if (turnPriorityFactor == -1) // if left turn is preferred
					   { 
						   if (!junction.gEast()) // if left is open
						   { // turn left

							   changeDirection(1);
							   return;
						   }
						   else
						   { // turn right

							   changeDirection(2);
							   return;
						   }
					   }
					   if (turnPriorityFactor != -1) // if right turn is preferred
					   {
						   if (!junction.gWest()) // if right is open
						   { // turn right

							   changeDirection(2);
							   return;
						   }
						   else
						   { // turn left

							   changeDirection(1);
							   return;
						   }
					   }
				   }
				   else // actually not needed
				   {
					   // go straight
				   }
					   
		} break;
		case 3:{ // east
				   // left preferred, unless turnPriorityFactor says right
				   if (!junction.gNorth()) // if left/north is open
				   {
					   if (turnPriorityFactor != 1)
					   { // turn left

						   changeDirection(1);
						   return;
					   }
				   }
				   if ((!junction.gEast()) && (!junction.gSouth())) // straight and right must be open
				   {
					   if (leftTurns > 2)
					   { // special case. only then would it prefer to go right. otherwise, go straight

						   // turn right
						   changeDirection(2);
					   }
					   else
					   {
						   // go straight
					   }
				   }
		} break;
		case 4:{ // west
				   // right/north is preferred
				   if (!junction.gNorth()) // if right/north is open
				   {
					   if (turnPriorityFactor != -1)
					   { // turn right

						   changeDirection(2);
						   return;
					   }
				   }
				   if ((!junction.gWest()) && (!junction.gSouth())) // straight and left must be open
				   {
					   if (rightTurns > 2)
					   { // special case. only then would it prefer to go left. otherwise, go straight

						   // turn left
						   changeDirection(1);
					   }
					   else
					   {
						   // go straight
					   }
				   }
		} break;
	default:
		break;
	}
}

// updates local view and calls motion functions
cell * star::motion(grid &fetched_scan, cell &currentcell, path &junctions, string &d, string &order)
{
	//path::iterator pathIt;
	cell *pathIterator;

	// start with motion forward so as not to get confused after being called once again to leave a cell
	//
	//
	if (order == "foward"){
		// constantly update the view as long as the mouse is moving
		while (true)
		{
			viewFinder.updateView(fetched_scan, leftFromViewfinder, rightFromViewfinder, frontFromViewfinder);


			/************** Check for dead end or finish position ***************/

			// dead end
			// might call a function instead...
			if (deadend)
			{
				cell *dead = new cell(1, 1, 1, direction);
				dead->deadend = true;
				return dead;
			}



			if ((leftFromViewfinder > threshold) && (rightFromViewfinder > threshold))
			{ // both sides are open. check front

				// if there is a wall in front
				if (frontFromViewfinder <= front_threshold)
				{
					cell *temp = new cell(0, 0, 1, direction);
					junctions.push_back(temp);

					return temp;
				}
				else // if there's no wall in front, still add the cell. the source direction is automatically blocked off by the constructor
				{
					cell *temp = new cell(0, 0, 0, direction);
					junctions.push_back(temp);

					return temp;
				}

				// motion forward , just to get to center


				//do{
				//	viewFinder.updateView(fetched_scan, leftFromViewfinder, rightFromViewfinder, frontFromViewfinder);
				//} while (frontFromViewfinder > threshold); // threshold for front should probably be the same value. if front does not exist and the sides start to reappear, it's a 4-way cell

			}
			else if ((leftFromViewfinder > threshold) && (rightFromViewfinder <= threshold))
			{	// left wall is open, right is not

				// if the front wall is open
				if (frontFromViewfinder > front_threshold)
				{
					cell *temp = new cell(0, 1, 0, direction);
					junctions.push_back(temp);

					return temp;
				}
				else if (frontFromViewfinder <= front_threshold)
				{	// there is a front wall. so simply turn left

					// move forward, 
					// turn left
					leftTurns++;
					rightTurns = 0;

					do{
						viewFinder.updateView(fetched_scan, leftFromViewfinder, rightFromViewfinder, frontFromViewfinder);
					} while (frontFromViewfinder > threshold); // threshold for front should probably be the same value. if front does not exist and the sides start to reappear, it's a 4-way cell

					// change direction
					changeDirection(1);
				}
			}
			else if ((leftFromViewfinder <= threshold) && (rightFromViewfinder > threshold))
			{	// right wall is open, left is not

				// if the front wall is open
				if (frontFromViewfinder > front_threshold)
				{
					cell *temp = new cell(1, 0, 0, direction);
					junctions.push_back(temp);

					return temp;
				}
				else if (frontFromViewfinder <= front_threshold)
				{	// simply turn right

					// move forward,
					// turn right
					rightTurns++;
					leftTurns = 0;

					do{
						viewFinder.updateView(fetched_scan, leftFromViewfinder, rightFromViewfinder, frontFromViewfinder);
					} while (frontFromViewfinder > threshold); // threshold for front should probably be the same value. if front does not exist and the sides start to reappear, it's a 4-way cell

					// change direction
					changeDirection(2);

					// only return when there's more than one choice at a cell... aka a "junction"
				}
			}
		}
	}
	else if (order == "backward") // going back. duhhhhh
	{
		// point to the last element in the junctions vector
		pathIterator = junctions.back();
		bool left, right, front; // these will change depending on the current direction. a function will return their values based on the last junction

		while (true)
		{
			viewFinder.updateView(fetched_scan, leftFromViewfinder, rightFromViewfinder, frontFromViewfinder);


			if ((leftFromViewfinder > threshold) && (rightFromViewfinder <= threshold))
			{	// left wall is open, right is not

				// if the front wall is open
				/** junction **/
				if (frontFromViewfinder > front_threshold)
				{
					// add a virtual wall to the cell
					pathIterator->markSourceDirection(direction);

					// check virtual walls
				}
				/** not a junction **/
				else if (frontFromViewfinder <= front_threshold)
				{	// there is a front wall. so simply turn left

					// move forward, 
					// turn left
					leftTurns++;
					rightTurns = 0;

					do{
						viewFinder.updateView(fetched_scan, leftFromViewfinder, rightFromViewfinder, frontFromViewfinder);
					} while (frontFromViewfinder > threshold); // threshold for front should probably be the same value. if front does not exist and the sides start to reappear, it's a 4-way cell

					// change direction
					changeDirection(1);
				}
			}
			else if ((leftFromViewfinder <= threshold) && (rightFromViewfinder > threshold))
			{	// right wall is open, left is not

				// if the front wall is open
				if (frontFromViewfinder > front_threshold)
				{
					cell *temp = new cell(1, 0, 0, direction);
					junctions.push_back(temp);

					return temp;
				}
				else if (frontFromViewfinder <= front_threshold)
				{	// simply turn right

					// move forward,
					// turn right
					rightTurns++;
					leftTurns = 0;

					do{
						viewFinder.updateView(fetched_scan, leftFromViewfinder, rightFromViewfinder, frontFromViewfinder);
					} while (frontFromViewfinder > threshold); // threshold for front should probably be the same value. if front does not exist and the sides start to reappear, it's a 4-way cell

					// change direction
					changeDirection(2);

					// only return when there's more than one choice at a cell... aka a "junction"
				}
			}
			else if ((leftFromViewfinder > threshold) && (rightFromViewfinder > threshold))
			{ // both sides are open. check front

				// if there is a wall in front
				if (frontFromViewfinder <= front_threshold)
				{
					cell *temp = new cell(0, 0, 1, direction);
					junctions.push_back(temp);

					return temp;
				}
				else // if there's no wall in front, still add the cell. the source direction is automatically blocked off by the constructor
				{
					cell *temp = new cell(0, 0, 0, direction);
					junctions.push_back(temp);

					return temp;
				}

				// motion forward , just to get to center


				//do{
				//	viewFinder.updateView(fetched_scan, leftFromViewfinder, rightFromViewfinder, frontFromViewfinder);
				//} while (frontFromViewfinder > threshold); // threshold for front should probably be the same value. if front does not exist and the sides start to reappear, it's a 4-way cell

			}
		}
	}

	return NULL;
}


int star::search(path &traversed, grid &fetched_scan)
{
	
	int result; // will determine course of action based on recursive returns

	cell *jCell; // pointer to the junction cell that's returned from motion

	jCell = motion(fetched_scan, current, traversed);

	if (jCell != NULL)
	{
		// the function will return a cell with either of these traits if it fits the conditions
		if (jCell->deadend == true)
			return -1;
		if (jCell->finish == true)
			return 1;

		// otherwise, treat the cell as a junction
		choose(*jCell);
	}

	result = search(traversed, fetched_scan);

	if (result == -1) // found a dead end
	{
		// turn around

		// call function to get back to previous junction

	}
	else if (result == 1)
	{
		// possibly move forward a bit

		return 1; // finish
	}

}


/********************************** starting fresh **********************************/

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

void star::createMaze()
{
	lat_headers.resize(mazeSize);
	long_headers.resize(mazeSize);

	int r, c;
	for (r = 0; r < mazeSize; r++)
	{
		for (c = 0; c < mazeSize; c++)
		{
			cell *temp = new cell(r, c);
			addCell(*temp);
		}
	}
}

void star::addCell(cell &newcell)
{
	cell *adder;
	adder = lat_headers[newcell.row];
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
		lat_headers[newcell.row] = &newcell;

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

void star::updateMaze(double x, double y)
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

	double previous_value;
	pI++;
	// create walls
	while (pI != vision.end())
	{
		if (((*pI)->aveRadius - (*follower)->aveRadius) > closeEnough)
		{
			
			int angle = (*follower)->angle - (*beginner)->angle; //angle that encompasses the wall from the viewpoint
			if (angle < 0)
				angle = -1 * angle;
			// length from beginning spot to the last spot that was recorded as part of the same wall
			double length;
			wall *nWall = new wall(length);

		}
		else
		{

		}


	}
}

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

void star::addBasedOnCompass(wall &wallInQuestion, string wallOrientation, double &x_displacement, double &y_displacement, double distanceToWall)
{
	double x_wall, y_wall;

	// assuming 90 is the starting direction. 0 is to the left at the beginning
	if ((compass > 315) && (compass < 45))
	{ // facing left
		x_wall = xDistance - y_displacement; // positive y displacement will be negative x
		y_wall = yDistance + x_displacement; // positive x is to the right, which will end up being positive y

		if ((wallOrientation == "perpendicularRight") || (wallOrientation == "perpendicularLeft")){
			x_wall -= wallInQuestion.length / 2;
		}
		else if (wallOrientation == "parallelLeft"){
			y_wall -= wallInQuestion.length / 2; // left is negative y
		}
		else if (wallOrientation == "parallelRight"){
			y_wall += wallInQuestion.length / 2;
		}
	}
	else if ((compass > 45) && (compass < 135)) 
	{ // default direction
		x_wall = xDistance + x_displacement;
		y_wall = yDistance + y_displacement;

		if ((wallOrientation == "perpendicularRight") || (wallOrientation == "perpendicularLeft")){
			y_wall += wallInQuestion.length / 2;
		}
		else if (wallOrientation == "parallelLeft"){
			x_wall -= wallInQuestion.length / 2;
		}
		else if (wallOrientation == "parallelRight"){
			x_wall += wallInQuestion.length / 2;
		}
	}
	else if ((compass > 135) && (compass < 225))
	{ // facing right
		x_wall = xDistance + y_displacement; // positive y will equal positve x
		y_wall = yDistance - x_displacement; // positive x is to the right, which will equal negative y

		if ((wallOrientation == "perpendicularRight") || (wallOrientation == "perpendicularLeft")){
			x_wall += wallInQuestion.length / 2;
		}
		else if (wallOrientation == "parallelLeft"){
			y_wall += wallInQuestion.length / 2;
		}
		else if (wallOrientation == "parallelRight"){
			y_wall -= wallInQuestion.length / 2; // right is negative y
		}
	}
	else if ((compass > 225) && (compass < 315))
	{ // facing south
		x_wall = xDistance - x_displacement; // reversed
		y_wall = yDistance - y_displacement; // reversed

		if ((wallOrientation == "perpendicularRight") || (wallOrientation == "perpendicularLeft")){
			y_wall -= wallInQuestion.length / 2;
		}
		else if (wallOrientation == "parallelLeft"){
			x_wall += wallInQuestion.length / 2; // left is positive x
		}
		else if (wallOrientation == "parallelRight"){
			x_wall -= wallInQuestion.length / 2; // right is negative x
		}
	}
	else if ((compass == 45) || (compass == 135) || (compass == 225) || (compass == 315))
	{

	}


}

bool closeEnough(double angle1, double angle2)
{
	if (abs(angle2 - angle1) <= 1.0)
		return true;
	else
		return false;
}
