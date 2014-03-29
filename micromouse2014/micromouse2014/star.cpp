//
//  star.cpp
//  micromouse2014
//
//

// This should be the only #include in the .cpp
#include "star.h"


#define cellsize 18
//#define testing 

/********** Star ********/
star::star(lidar & the_lidar, nav & the_nav):
lide(the_lidar), navigator(the_nav)
{
    deadend = false;
    
#ifndef testing
	maze.direction = "north";
	maze.compass = 90; // starting position is 90 degrees, which will be "north". 0 degrees is to the left to match up with the lidar
	maze.shift = 0.0; // default position, so shift is 0 degrees
    
	maze.startingRow = 0;
	maze.startingColumn = 0;
	atJunction = false;


	// The goal cell's attributes, assuming the goal is always the center
	maze.goalX = (maze.mazeSize / 2) * cellsize;	// halfway through the maze, multiplied by the centimeters in each cell
	maze.goalY = maze.goalX;

	rightTurns = 0;
	leftTurns = 0;

	// create the maze
	maze.createMaze();	
#endif 
}

void star::theLoop()
{
	std::map<uint, const data*>::iterator dIterate;

	cell *current;
	while (true)
	{
		lide.build_scan();
		//auto deqIterate = lide.scan_hist.begin(); // deque iterator for 360 scan history. points to whole scans. begin is the latest
		//auto degreeIt = (**deqIterate).begin; // degree iterator
		////(**deqIterate).deg_index[]; // how to access the individual degree
		//
		scan();
		maze.updateMaze();
		
		// move to an upcoming junction, scan & update maze some more
		decide();
		// update heuristic values
		breadthSearch();
		// decide which path to do
		depthSearch(1);

		current = maze.findCell(maze.xDistance, maze.yDistance);
		if (current->goalCell)
			break;
	}
}

bool closeEnough(double angle1, double angle2)
{
	if (abs(angle2 - angle1) <= 1.0)
		return true;
	else
		return false;
}


void polarToCartesian(double radius, double angle, double &x, double &y)
{
	x = radius * cos(angle);
	y = radius * sin(angle);
}

void star::scan()
{
//#ifdef testing
    using namespace star_config;
	
	// subject to change
	double closeEnough = 1;	// in millimeters

	auto _360_it = lide.build_scan(); // create new scan
	auto deqIterate = lide.scan_hist.begin(); // deque iterator for 360 scan history. points to whole scans. begin is the latest
	//auto deg_it  = (**deqIterate).deg_index.begin(); // degree iterator

	map<uint, const data*>::iterator beginner;
	beginner = (**deqIterate).deg_index.begin();
	while (beginner->second->invalid_data)
	{
		beginner++;
	}

	map<uint, const data*>::iterator pI;
	pI = beginner;
	
	map<uint, const data*>::iterator follower;
	follower = beginner;

	int i = 0;
	string wallOrientation;
	double wallDisplacement_x, wallDisplacement_y, distanceToWall;
	double previous_value;
	

	// pI is the ahead iterator. follower is right behind it. beginner is the one that 
	// is at the beginning of the current wall
	pI++;
	uint difference;
	int direction = 0; // 1 = increasing distances, -1 = decreasing distances
	int initialDirection;

// prepare to go forward anyway
	int countSuccess;

	// check direction for the first time, using initialDirection
	if (!(*pI).second->invalid_data && !(*follower).second->invalid_data)
		difference = (*pI).second->distance - (*follower).second->distance; // the difference between the follower's distance and the ahead distance 
	directionOfMeasurementChanges(difference, initialDirection);


	while ((*pI).second->invalid_data || (*follower).second->invalid_data)
	{
		pI++;
		follower++;
	}

	while (pI != (**deqIterate).deg_index.end())
	{
		// check direction
		if (!(*pI).second->invalid_data && !(*follower).second->invalid_data)
		{
			difference = (*pI).second->distance - (*follower).second->distance; // the difference between the follower's distance and the ahead distance 
			directionOfMeasurementChanges(difference, direction);

			if ((direction != initialDirection) || (abs((*pI).second->distance - (*follower).second->distance) > closeEnough))
			{	// create a new wall if the direction of change in measurement switches, or if the difference in measurements is greater than closeEnough

				int angle = (*follower).first - (*beginner).first; //angle that encompasses the wall from the viewpoint
				if (angle < 0)
					angle = -1 * angle;
				// length from beginning spot to the last spot that was recorded as part of the same wall
				double length;
				// create the wall, orient it, and add it to the maze
				wall *nWall = new wall((*beginner).second->distance, (*follower).second->distance, (*beginner).first, (*follower).first);
				maze.wallOrienter(*nWall, wallOrientation, wallDisplacement_x, wallDisplacement_y, distanceToWall);
				maze.addBasedOnCompass(*nWall, wallOrientation, wallDisplacement_x, wallDisplacement_y, distanceToWall);

				pI++; // the ahead iterator
				follower++;
				beginner = follower; // set the beginner to the new wall
			}
			else
			{
				follower++;
				pI++;
			}
		}
		else
		{
			follower++;
			pI++;
		}
	}


//#endif
}


/* return true if there's a new wall. 
This is determined by iterating through the 360 degrees and 
seeing where the "derivative" changes sign. aka, increasing vs. decreasing*/
/*bool star::newWalleXists()
{
	auto _360_it = lide.build_scan(); // create new scan
	auto deqIterate = lide.scan_hist.begin(); // deque iterator for 360 scan history. points to whole scans. begin is the latest
	//auto deg_it  = (**deqIterate).deg_index.begin(); // degree iterator

	// prepare to iterate through the latest scan
	map<uint, const data*>::iterator pI;
	pI = (**deqIterate).deg_index.begin();
	map<uint, const data*>::iterator beginner;
	beginner = (**deqIterate).deg_index.begin();
	map<uint, const data*>::iterator follower;
	follower = (**deqIterate).deg_index.begin();

	// pI is the ahead iterator. follower is right behind it. beginner is the one that 
	// is at the beginning of the current wall
	pI++;

	// this deque will hold the last 5, and as soon as it notices that the values are no longer increasing/decreasing, it declares a new wall
	//std::deque<uint> checker;

	uint difference;
	int direction; // 1 = increasing distances, -1 = decreasing distances
	int initialDirection;

	// check direction for the first time, using initialDirection
	difference = (*pI).second->distance - (*follower).second->distance; // the difference between the follower's distance and the ahead distance 
	directionOfMeasurementChanges(difference, initialDirection);

	while (pI != (**deqIterate).deg_index.end())
	{
		// check direction
		difference = (*pI).second->distance - (*follower).second->distance; // the difference between the follower's distance and the ahead distance 
		directionOfMeasurementChanges(difference, direction);

		if (direction != initialDirection)
		{
			return true;
		}
		
		follower++;
		pI++;
	}

	return false;
}*/

// says whether the direction of measurement differences is increasing/decreasing.
// called by scan()
void star::directionOfMeasurementChanges(uint difference, int &direction)
{
	if (difference > 0)
	{	// the distances are increasing
		direction = 1;
	}
	else if (difference < 0)
	{	// the distances are decreasing
		direction = -1;
	}
	else
	{	// the distances are the same
		direction = 0; // not likely to happen, but must include for the sake of including
	}
}







/*********** movement **********/




// decision-making at junctions. updates the map with new scans from the middle of the junction.
// does a breadth search to guess which path is faster, and gives movement costs based on this search.
// Then a breadth search from the goal to the current position is performed, updating the heuristic costs
int star::decide()
{
#ifndef testing
	using namespace lidar_config;

	uint16_t moveDistance, xDifference, yDifference;
	cell *currentCell = maze.findCell(maze.xDistance, maze.yDistance);

	/******************* Find the next upcoming junction *************************/
	char sourceDirection;
	cell *cellPoint = maze.getPointerToJunction(sourceDirection); // return sourceDirection


	// Figure out how far the robot must move to get to the center of the upcoming junction
	xDifference = abs(cellPoint->x_center - maze.xDistance);
	yDifference = abs(cellPoint->y_center - maze.yDistance);


	// choose which difference to use; x or y
	switch (sourceDirection)
	{
	case 'n':	// use the y since the robot is facing in that orientation
	case 's':
		moveDistance = yDifference;
		break;
	case 'e': // use the x since the robot is oriented that way
	case 'w':
		moveDistance = xDifference;
		break;
	default:
		break;
	}


	// An upcoming junction has been found; move to it
	if (cellPoint->declareSidesOpen(sourceDirection)) 
	{
		// latest 360 scan
		auto scanInitial = lide.scan_hist.front();
		
		// check the distance of the wall in front, before moving
		auto pack1 = scanInitial->deg_index.at(degree_north);
		uint16_t distance1 = pack1->distance;

		/***** MOVE THIS MUCH *****/
		navigator.movedistancevariable(moveDistance);

		// update the position after checking the real change in distance

/**************************

	SLEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEP

**************************************************/

		// scan again. the difference between the scanned distances for the front 
		//	should be equal to the calculated moveDistance.
		auto scanSecond = lide.scan_hist.front();	// second scan
		auto pack2 = scanSecond->deg_index.at(degree_north);
		uint16_t distance2 = pack2->distance;

		// get the difference between the two distances; cast them to int,
		// so that the result can be signed rather than unsigned
		int displacement_actual = int(distance1) - int(distance2);

		// updating the position accordingly
		PositionChange(displacement_actual);

		// fine tune the distance
		int correctedDistance = abs(displacement_actual - int(moveDistance));

		// go backward just a bit
		if (displacement_actual > int(moveDistance))
		{
			// move backward to make up the difference
			navigator.movedistancevariable(-correctedDistance);
			
			// update position
			PositionChange(-correctedDistance);
		}
		// go forward just a bit
		else
		{
			// move forward to make up the difference
			navigator.movedistancevariable(correctedDistance);

			// update position
			PositionChange(correctedDistance);
		}

		// declare that we are at a junction. inside the junction. Don't scan. Turn first
		atJunction = true;
	}
	else
	{
		// just go forward
		auto scannnn = lide.scan_hist.front();
		auto frontpack = scannnn->deg_index.at(degree_north);
		moveDistance = frontpack->distance - 0.5 * nav_config::cell_size;
		navigator.movedistancevariable(moveDistance);
		PositionChange(moveDistance);
	}

	// update the maze again so that when breadth & depth searches are called, they have more information
	if (atJunction == true)
	{
		// no need to turn, just check latest scan to see and update the grid even MORE
		// READ SCAN
		scan();
		maze.updateMaze();
    }
	else
	{
		scan();
		maze.updateMaze();
	}

#endif
	return 0;
}

// called after movement, with actual measurements from lidar. 
// the value is going to positive because this is when the robot is moving forward, 
// thus the initial measurement - the second measurement is going to be positive
void star::PositionChange(double displacement_Actual)
{
	double distance = displacement_Actual;

	if ((compass > 315) && (compass < 45))
	{ // facing left
		maze.xDistance -= distance;
	}
	else if ((compass > 45) && (compass < 135))
	{ // default direction
		maze.yDistance += distance;
	}
	else if ((compass > 135) && (compass < 225))
	{ // facing right
		maze.xDistance += distance;
	}
	else if ((compass > 225) && (compass < 315))
	{ // facing south
		maze.yDistance -= distance;
	}
	else if ((compass == 45) || (compass == 135) || (compass == 225) || (compass == 315))
	{
		// hmmmm......

		// possibly a turn instruction
	}

}

void star::pushChildCellsToDeque(std::deque<cell*> &childCells)
{

}

void star::breadthSearch()
{
	int countCost = 1; // the movement cost increments every time you get to a new set of children
	cell *currentCell = maze.findCell(maze.xDistance, maze.yDistance);


	std::deque<cell*> childCells; // used for the breadth search, contains the current child cells. queue is not supported for some reason, so use in the same way. push_back, pop front



	/********** CLEAR CELL VALUES ***********/
	// actually, they'll be overwritten

	/****************************************/



	// look at child cells
	cell *cellP = currentCell;
	int north, south, east, west;
	cellP->returnSides(north, south, east, west);

	int i = 1;

	// add any cell as long as there's no wall between it and the current cell pointer
	// add their movement costs
	do{
		if (north < 1)
		{
			if (!cellP->north->goalCell)
			{
				cellP->north->movementCost = countCost;
				cellP->north->sourceDirection = 'n';
				childCells.push_back(cellP->north);
			}
		}
		if (west < 1)
		{
			if (!cellP->west->goalCell)
			{
				cellP->west->movementCost = countCost;
				cellP->west->sourceDirection = 'w';
				childCells.push_back(cellP->west);
			}
		}
		if (south < 1)
		{
			if (!cellP->south->goalCell)
			{
				cellP->south->movementCost = countCost;
				cellP->south->sourceDirection = 's';
				childCells.push_back(cellP->south);
			}
		}
		if (east < 1)
		{
			if (!cellP->east->goalCell)
			{
				cellP->east->movementCost = countCost;
				cellP->east->sourceDirection = 'e';
				childCells.push_back(cellP->east);
			}
		}

		// move the pointer to one of the next child cells in the 'queue'
		cellP = childCells.front();
		childCells.pop_front(); // remove the child cell from the queue

		// return the sides of the child cell excluding the source side
		cellP->returnSides(north, south, east, west, cellP->sourceDirection);

		i++;
		countCost++;
	} while ((i < ((maze.mazeSize*maze.mazeSize) - 4)));	// do every cell except the goal cell


	//give the cells their new heuristic cost *************************
	// WORKING from the goal to the current position

	// look at child cells
	double xxxx = cellP->x_center, yyyy = cellP->y_center;
	cellP = maze.findClosestGoalCell(xxxx, yyyy);
    
	//why is this here?
    //int north, south, east, west;
	cellP->returnSides(north, south, east, west);

	countCost = 1;
	i = 1;

	// add any cell as long as there's no wall between it and the current cell pointer
	// add their movement costs
	do{
		if (north < 1)
		{
			if (!cellP->north->goalCell)
			{
				cellP->north->heuristicCost = countCost;
				cellP->north->sourceDirection = 'n';
				childCells.push_back(cellP->north);
			}
		}
		if (west < 1)
		{
			if (!cellP->west->goalCell)
			{
				cellP->west->heuristicCost = countCost;
				cellP->west->sourceDirection = 'w';
				childCells.push_back(cellP->west);
			}
		}
		if (south < 1)
		{
			if (!cellP->south->goalCell)
			{
				cellP->south->heuristicCost = countCost;
				cellP->south->sourceDirection = 's';
				childCells.push_back(cellP->south);
			}
		}
		if (east < 1)
		{
			if (!cellP->east->goalCell)
			{
				cellP->east->heuristicCost = countCost;
				cellP->east->sourceDirection = 'e';
				childCells.push_back(cellP->east);
			}
		}

		// move the pointer to one of the next child cells in the 'queue'
		cellP = childCells.front();
		childCells.pop_front(); // remove the child cell from the queue

		// return the sides of the child cell excluding the source side
		cellP->returnSides(north, south, east, west, cellP->sourceDirection);

		i++;
		countCost++;
	} while ((i < ((maze.mazeSize*maze.mazeSize) - 4)));	// do every cell except the current cell

}

// mode 1 = go with the "known" path
// mode 2 = go with the "unknown" path
void star::depthSearch(int mode)
{
	// start at current cell once again
	cell *current = maze.findCell(maze.xDistance, maze.yDistance);
	// prepare to check walls
	int north, south, east, west;

	// check for dead end. if it's a dead end, return
	current->returnSides(north, south, east, west);
	int walls = north + south + east + west;
	/*//if (walls >= 3)
	//{
	//	// turn around
	//	turn()
	//	return;	// dead end
	//}*/

	current->previousCell = current; // take care of the previous cell pointer

	/***** container for the possible paths *****/
	vector<path*> possiblePaths;

	path *chosenPath;

	switch (mode)
	{
	case 1:{	// go with the known path
			   if (north < 1)
			   {
				   if (current->north->returnSum() <= current->returnSum()) // a dead end will have a sum greater than the current cell
				   {
					   path *p1 = new path();
					   cell *c1;
					   c1 = current->nextCell(); // move to next cell and mark its previouscell as this current one
					   p1->members.push_back(c1);

					   while (true)
					   {	// go to the child cells and add cells with the same sum to the path
						   c1 = c1->nextCell();
						   p1->members.push_back(c1);
						   p1->unknownWalls += c1->numUnknownSides(); // count the unknown sides

						   if (c1->goalCell)
							   break;
					   }
					   possiblePaths.push_back(p1);
				   }
			   }
			   if (south < 1)
			   {
				   if (current->south->returnSum() <= current->returnSum()) // a dead end will have a sum greater than the current cell
				   {
					   path *p2 = new path();
					   cell *c2 = current->nextCell();
					   p2->members.push_back(c2);
					   
					   while (true)
					   {
						   c2 = c2->nextCell();
						   p2->members.push_back(c2);
						   p2->unknownWalls += c2->numUnknownSides();

						   if (c2->goalCell)
							   break;
					   }

					   possiblePaths.push_back(p2);
				   }
			   }
			   if (east < 1)
			   {
				   if (current->east->returnSum() <= current->returnSum()) // a dead end will have a sum greater than the current cell
				   {
					   path *p3 = new path();
					   cell *c3 = current->nextCell();
					   p3->members.push_back(c3);

					   while (true)
					   {
						   c3 = c3->nextCell();
						   p3->members.push_back(c3);
						   p3->unknownWalls += c3->numUnknownSides(); // count the unknown sides

						   if (c3->goalCell)
							   break;
					   }

					   possiblePaths.push_back(p3);
				   }
			   }
			   if (west < 1)
			   {
				   if (current->west->returnSum() <= current->returnSum()) // a dead end will have a sum greater than the current cell
				   {
					   path *p4 = new path();
					   cell *c4 = current->nextCell();
					   p4->members.push_back(c4);

					   while (true)
					   {
						   c4 = c4->nextCell();
						   p4->members.push_back(c4);
						   p4->unknownWalls += c4->numUnknownSides();

						   if (c4->goalCell)
							   break;
					   }

					   possiblePaths.push_back(p4);
				   }
			   }

			   // choose the path with the least unknowns. just do walls in general for now
			   int lowestIndex = 0;
			   for (int q = 0; q < possiblePaths.size(); q++)
			   {
				   if (possiblePaths[q]->unknownWalls < possiblePaths[lowestIndex]->unknownWalls)
					   lowestIndex = q;
			   }
			   // choose the path
			   chosenPath = possiblePaths[lowestIndex];

			   // switch statement
			   switch (lowestIndex)
			   {
			   case 0:{
						  // go north
						  turn(0);
			   }
				   break;
			   case 1:{
						  // go south
						  turn(1);
			   }
				   break;
			   case 2:{
						  // go east
						  turn(2);
			   }
				   break;
			   case 3:{
						  // go west
						  turn(3);
			   }
				   break;
			   default:
				   break;
			   }
			   		
	}	break;
	case 2:{	// go with the most unknown path
			   if (north < 1)
			   {
				   if (current->north->returnSum() <= current->returnSum()) // a dead end will have a sum greater than the current cell
				   {
					   path *p1 = new path();
					   cell *c1;
					   c1 = current->nextCell();
					   p1->members.push_back(c1);

					   while (true)
					   {	// go to the child cells and add cells with the same sum to the path
						   c1 = c1->nextCell();
						   p1->members.push_back(c1);
						   p1->unknownWalls += c1->numUnknownSides(); // count the unknown sides

						   if (c1->goalCell)
							   break;
					   }
					   possiblePaths.push_back(p1);
				   }
			   }
			   if (south < 1)
			   {
				   if (current->south->returnSum() <= current->returnSum()) // a dead end will have a sum greater than the current cell
				   {
					   path *p2 = new path();
					   cell *c2 = current->nextCell();
					   p2->members.push_back(c2);

					   while (true)
					   {
						   c2 = c2->nextCell();
						   p2->members.push_back(c2);
						   p2->unknownWalls += c2->numUnknownSides();

						   if (c2->goalCell)
							   break;
					   }

					   possiblePaths.push_back(p2);
				   }
			   }
			   if (east < 1)
			   {
				   if (current->east->returnSum() <= current->returnSum()) // a dead end will have a sum greater than the current cell
				   {
					   path *p3 = new path();
					   cell *c3 = current->nextCell();
					   p3->members.push_back(c3);

					   while (true)
					   {
						   c3 = c3->nextCell();
						   p3->members.push_back(c3);
						   p3->unknownWalls += c3->numUnknownSides(); // count the unknown sides

						   if (c3->goalCell)
							   break;
					   }

					   possiblePaths.push_back(p3);
				   }
			   }
			   if (west < 1)
			   {
				   if (current->west->returnSum() <= current->returnSum()) // a dead end will have a sum greater than the current cell
				   {
					   path *p4 = new path();
					   cell *c4 = current->nextCell();
					   p4->members.push_back(c4);

					   while (true)
					   {
						   c4 = c4->nextCell();
						   p4->members.push_back(c4);
						   p4->unknownWalls += c4->numUnknownSides();

						   if (c4->goalCell)
							   break;
					   }

					   possiblePaths.push_back(p4);
				   }
			   }

			   // choose the path with the most unknowns. just do walls in general for now
			   int highestIndex = 0;
			   for (int h = 0; h < possiblePaths.size(); h++)
			   {
				   if (possiblePaths[h]->unknownWalls > possiblePaths[highestIndex]->unknownWalls)
					   highestIndex = h;
			   }
			   // choose path
			   chosenPath = possiblePaths[highestIndex];

			   // switch statement
			   switch (highestIndex)
			   {
			   case 0:{
						  // go north
						  turn(0);
			   }
				   break;
			   case 1:{
						  // go south
						  turn(1);
			   }
				   break;
			   case 2:{
						  // go east
						  turn(2);
			   }
				   break;
			   case 3:{
						  // go west
						  turn(3);
			   }
				   break;
			   default:
				   break;
			   }

	}
	default:
		break;
	}

}

// turn to a certain side. 0 = north. 1 = south. 2 = east. 3 = west
void star::turn(int direction)
{
	// assuming 90 is the starting direction. 0 is to the left at the beginning
	if ((maze.compass > 315) && (maze.compass < 45))
	{ // facing west
		switch (direction)
		{
		case 0:	// turn right
			navigator.turnright();
			maze.adjustCompass(90);
			break;
		case 1:	// turn left
			navigator.turnleft();
			maze.adjustCompass(-90);
			break;
		case 2:	// do 180 degree turn
			navigator.turnaround();
			maze.adjustCompass(180);
			break;
		case 3:	// do nothing
			break;
		default:
			break;
		}
	}
	else if ((maze.compass > 45) && (maze.compass < 135))
	{ // facing north
		switch (direction)
		{
		case 0:	// do nothing
			break;
		case 1:	// do 180 degree turn
			navigator.turnaround();
			maze.adjustCompass(180);
			break;
		case 2:	// turn right
			navigator.turnright();
			maze.adjustCompass(90);
			break;
		case 3:	// turn left
			navigator.turnleft();
			maze.adjustCompass(-90);
			break;
		default:
			break;
		}
	}
	else if ((maze.compass > 135) && (maze.compass < 225))
	{ // facing east
		switch (direction)
		{
		case 0:	// turn left
			navigator.turnleft();
			maze.adjustCompass(-90);
			break;
		case 1:	// turn right
			navigator.turnright();
			maze.adjustCompass(90);
			break;
		case 2:	// do nothing
			break;
		case 3:	// do 180 degree turn
			navigator.turnaround();
			maze.adjustCompass(180);
			break;
		default:
			break;
		}
	}
	else if ((maze.compass > 225) && (maze.compass < 315))
	{	// facing south
		switch (direction)
		{
		case 0:	// do 180 degree turn
			navigator.turnaround();
			maze.adjustCompass(180);
			break;
		case 1:	// do nothing
			break;
		case 2:	// turn right
			navigator.turnright();
			maze.adjustCompass(90);
			break;
		case 3:	// turn left
			navigator.turnleft();
			maze.adjustCompass(-90);
			break;
		default:
			break;
		}
	}
	else if ((maze.compass == 45) || (maze.compass == 135) || (maze.compass == 225) || (maze.compass == 315))
	{
		// hmmmm......

		// possibly a turn instruction
	}
}

/*(used in depthSearch)
return the adjacent cell that has the same sum as the current cell, meaning it's the next cell in the path.
exclude the previous cell
*/
cell *star::nextCellinPath(cell &current)
{
    cell *chosen;
	int north, south, east, west;
    current.returnSides(north,south,east,west);

	if ((north < 1) && (current.north != current.previousCell))
	{
		if ((current.north->returnSum() == current.returnSum()) || (current.north->goalCell))
			chosen = current.north;

		if (current.north->goalCell)
			chosen = current.north;
	}
	if ((south < 1) && (current.south != current.previousCell))
	{
		if ((current.south->returnSum() == current.returnSum()) || (current.south->goalCell))
			chosen = current.south;
	}
	if ((east < 1) && (current.east != current.previousCell))
	{
		if ((current.east->returnSum() == current.returnSum()) || (current.east->goalCell))
			chosen = current.east;
	}
	if ((west < 1) && (current.west != current.previousCell))
	{
		if ((current.west->returnSum() == current.returnSum()) || (current.west->goalCell))
			chosen = current.west;
	}
    return chosen;
}

void star::determineMovementCost(cell &ce)
{
	cell * cellIt;
	cellIt = maze.findCell(0, 0);


	// this isn't used, i don't think
}

// find the initial heuristic cost for all of the cells
void star::determineheuristicCost()
{
	// it's okay to measure from the exact center rather than the range of the cell in the center
	maze.goalX = maze.mazeSize * cellsize * 0.5;
	maze.goalY = maze.goalX;


}




cell * 
star::getPointerToJunction(char &sourceDirection)
{
	// get the current cell in which the robot is located
	cell * currentCell = maze.findCell(maze.xDistance, maze.yDistance);
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


