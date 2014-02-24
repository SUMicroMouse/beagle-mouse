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
	compass = 0;
	num_packets = 90;

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
				packet *pack = new packet(nums[0], nums[1], nums[2], nums[3]); 
				nums.empty();
				vision.push_back(pack);
			}
		}
	}
}

void star::updateMaze(double x, double y)
{
	// add walls to wall deque. probably should turn in the direction of the longer wall to record the pathway in full

	packets::iterator pI; // iterate through vector of packets
	pI = vision.begin();
	packets::iterator follower;
	follower = vision.begin();

	int i = 0;

	pI++;
	// create walls
	while (pI != vision.end())
	{


	}
}

