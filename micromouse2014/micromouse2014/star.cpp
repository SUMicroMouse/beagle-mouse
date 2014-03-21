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
#include <iostream>

#define cellsize 16
bool deadend = false;




/********** Star ********/
star::star()
{
	
	direction = "north";
	compass = 90; // starting position is 90 degrees, which will be "north". 0 degrees is to the left to match up with the lidar
	shift = 0.0; // default position, so shift is 0 degrees
    
	startingRow = 0;
	startingColumn = 0;
	atJunction = false;


	// The goal cell's attributes, assuming the goal is always the center
	goalX = (mazeSize / 2) * cellsize;	// halfway through the maze, multiplied by the centimeters in each cell
	goalY = goalX;

	rightTurns = 0;
	leftTurns = 0;

	// create the maze
	createMaze();

	/********* Give the cells their heuristicCost, the estimated cost to the goal ******/


	// scan and update the maze
	while (true)
	{
		scan();	headOnDistance = -1; // set headOnDistance to the average of the packets that represent what is directly in front. compensate with compass
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


void polarToCartesian(double radius, double angle, double &x, double &y)
{
	x = radius * cos(angle);
	y = radius * sin(angle);
}

void star::scan()
{
    using namespace star_config;
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
				packet *pack = 
				nums.empty();
				vision.push_back(pack);
			}
		}
	}
}





/*********** movement **********/




// decision-making at junctions. updates the map with new scans from the middle of the junction.
// does a breadth search to guess which path is faster, and gives movement costs based on this search.
// Then a breadth search from the goal to the current position is performed, updating the heuristic costs
int star::decide()
{
	double moveDistance;
	cell *currentCell = findCell(xDistance, yDistance);

	/******************* Find the next upcoming junction *************************/
	char sourceDirection;
	cell *cellPoint = getPointerToJunction(sourceDirection);

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
			goForwardOne();

		// declare that we are at a junction. inside the junction. Don't scan. Turn first
		atJunction = true;
	}

	
	if (atJunction == true)
	{
		// no need to turn, just check latest scan to see and update the grid even MORE
		// READ SCAN
		scan();
		updateMaze();
    }
	return 0;
}

void star::pushChildCellsToDeque(std::deque<cell*> &childCells)
{

}

void star::breadthSearch()
{
	int countCost = 1; // the movement cost increments every time you get to a new set of children
	cell *currentCell = findCell(xDistance, yDistance);
	
	
	std::deque<cell*> childCells; // used for the breadth search, contains the current child cells. queue is not supported for some reason, so use in the same way. push_back, pop front



	/********** CLEAR CELL VALUES ***********/





	// look at child cells
	cell *cellP = currentCell;
	int north, south, east, west;
	cellP->returnSides(north, south, east, west);


	// add any cell as long as there's no wall between it and the current cell pointer
	// add their movement costs
	do{
		if (north < 1)
		{
			cellP->north->movementCost = countCost;
			cellP->north->sourceDirection = 'n';
			childCells.push_back(cellP->north);
		}
		if (west < 1)
		{
			cellP->west->movementCost = countCost;
			cellP->west->sourceDirection = 'w';
			childCells.push_back(cellP->west);
		}
		if (south < 1)
		{
			cellP->south->movementCost = countCost;
			cellP->south->sourceDirection = 's';
			childCells.push_back(cellP->south);
		}
		if (east < 1)
		{
			if ()
			cellP->east->movementCost = countCost;
			cellP->east->sourceDirection = 'e';
			childCells.push_back(cellP->east);
		}

		// move the pointer to one of the next child cells in the 'queue'
		cellP = childCells.front();
		childCells.pop_front(); // remove the child cell from the queue

		// return the sides of the child cell excluding the source side
		cellP->returnSides(north, south, east, west, cellP->sourceDirection);

	} while (!cellP->goalCell);


	//give the cells their new heuristic cost

}



void star::determineMovementCost(cell &ce)
{
	cell * cellIt;
	cellIt = findCell(0, 0);



}

// find the initial heuristic cost for all of the cells
void star::determineheuristicCost()
{
	// it's okay to measure from the exact center rather than the range of the cell in the center
	goalX = mazeSize * cellsize * 0.5;
	goalY = goalX;


}



