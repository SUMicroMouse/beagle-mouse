//
//  star.cpp
//  micromouse2014
//
//

// This should be the only #include in the .cpp
#include "star.h"


#define cellsize 16
//#define testing 

/********** Star ********/
star::star(lidar & the_lidar, nav & the_nav):
view(the_lidar), navigator(the_nav)
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

	/********* Give the cells their heuristicCost, the estimated cost to the goal ******/


	// scan and update the maze
	while (true)
	{
		scan();	headOnDistance = -1; // set headOnDistance to the average of the packets that represent what is directly in front. compensate with compass
		maze.updateMaze();
		//goForwardOne();
		scan();	headOnDistance2 = -1;
		maze.updateMaze();
		rob.PositionChange();

		decide(); // decide will check current spot, choose to go forward, left, right, blah blah
	}

	
#endif 
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
#ifdef testing
    using namespace star_config;
	vision = view.build_scan(); // empty the previous vision. new scan
    
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
				packet *pack = 
				nums.empty();
				vision.push_back(pack);
			}
		}
	}
#endif
}





/*********** movement **********/




// decision-making at junctions. updates the map with new scans from the middle of the junction.
// does a breadth search to guess which path is faster, and gives movement costs based on this search.
// Then a breadth search from the goal to the current position is performed, updating the heuristic costs
int star::decide()
{
#ifndef testing
	double moveDistance;
	cell *currentCell = maze.findCell(maze.xDistance, maze.yDistance);

	/******************* Find the next upcoming junction *************************/
	char sourceDirection;
	cell *cellPoint = maze.getPointerToJunction(sourceDirection);

		int rowDifference, columnDifference, difference;
		// difference is either in columns or rows
			rowDifference = abs(currentCell->row - cellPoint->row);		columnDifference = abs(currentCell->column - cellPoint->column);
		if (rowDifference > 0)
			difference = rowDifference;
		else if (columnDifference > 0)
			difference = columnDifference;

	// An upcoming junction has been found; move to it
	if (cellPoint->declareSidesOpen(sourceDirection))
	{
		// get to the middle of the junction, as in inside the junction itself
		moveDistance = cellsize * difference;





		/***** MOVE THIS MUCH *****/
		for (int i = 0; i < moveDistance; i++)
			na.goForwardOne();

		// declare that we are at a junction. inside the junction. Don't scan. Turn first
		atJunction = true;
	}



	
	if (atJunction == true)
	{
		// no need to turn, just check latest scan to see and update the grid even MORE
		// READ SCAN
		scan();
		maze.updateMaze();
    }

#endif

	return 0;
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
	cellP = maze.findClosestGoalCell(cellP->x_center, cellP->y_center);
    
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







	/********* depth search ***********/

	
	


}

// mode 1 = go with the "known" path
// mode 2 = go with the "unknown" path
int star::depthSearch(cell &sender, std::stack<cell*> &tempStack, std::deque<cell*> &pathKnown, std::deque<cell*> &pathUnknown, int &unknownSides, int mode)
{
	// start at current cell once again
	cell *current = maze.findCell(maze.xDistance, maze.yDistance);
	// prepare to check walls
	int north, south, east, west;

	// check for dead end. if it's a dead end, return
	current->returnSides(north, south, east, west);
	int walls = north + south + east + west;
	if (walls >= 3)
		return -1;	// dead end


	/*cell *closestGoalCell = maze.findClosestGoalCell(current->x_center, current->y_center);
	double x = current->x_center - closestGoalCell->x_center;
	double y = current->y_center - closestGoalCell->y_center;
	double distance = sqrt((x*x) + (y*y));
*/

	current->previousCell = current; // take care of the previous cell pointer

    //These two vars are unused.
	//double lowestSum;
	//cell *lowestNeighbor; // add the neighbor with the lowest sum to the path

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

			   // choose the path with the least unknowns. just do walls in general for now
			   int lowestIndex = 0;
			   if (possiblePaths[1]->unknownWalls < possiblePaths[0]->unknownWalls)
				   lowestIndex = 1;
			   if (possiblePaths[2]->unknownWalls < possiblePaths[1]->unknownWalls)
				   lowestIndex = 2;
			   if (possiblePaths[3]->unknownWalls < possiblePaths[2]->unknownWalls)
				   lowestIndex = 3;

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

			   // choose the path with the least unknowns. just do walls in general for now
			   int highestIndex = 0;
			   if (possiblePaths[1]->unknownWalls > possiblePaths[0]->unknownWalls)
				   highestIndex = 1;
			   if (possiblePaths[2]->unknownWalls > possiblePaths[1]->unknownWalls)
				   highestIndex = 2;
			   if (possiblePaths[3]->unknownWalls > possiblePaths[2]->unknownWalls)
				   highestIndex = 3;

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
			break;
		case 1:	// turn left
			break;
		case 2:	// do 180 degree turn
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
			break;
		case 2:	// turn right
			break;
		case 3:	// turn left
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
			break;
		case 1:	// turn right
			break;
		case 2:	// do nothing
			break;
		case 3:	// do 180 degree turn
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
			break;
		case 1:	// do nothing
			break;
		case 2:	// turn right
			break;
		case 3:	// turn left
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
	int north, south, east, west;

	if ((north < 1) && (current.north != current.previousCell))
	{
		if ((current.north->returnSum() == current.returnSum()) || (current.north->goalCell))
			return current.north;

		if (current.north->goalCell)
			return current.north;
	}
	if ((south < 1) && (current.south != current.previousCell))
	{
		if ((current.south->returnSum() == current.returnSum()) || (current.south->goalCell))
			return current.south;
	}
	if ((east < 1) && (current.east != current.previousCell))
	{
		if ((current.east->returnSum() == current.returnSum()) || (current.east->goalCell))
			return current.east;
	}
	if ((west < 1) && (current.west != current.previousCell))
	{
		if ((current.west->returnSum() == current.returnSum()) || (current.west->goalCell))
			return current.west;
	}
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




