//
//  star.cpp
//  micromouse2014
//
//

#include "star.h"

local_grid::local_grid()
{
	viewfinder = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	previous = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
}

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
	// size of cells
	lengthwidth = 16; // free to change
	threshold = 2;

	leftFromViewfinder = 0;
	rightFromViewfinder = 0;
	frontFromViewfinder = 0;
}


void star::motion(grid &fetched_scan)
{
	// constantly update the view as long as the mouse is moving
	while (true)
	{
		viewFinder.updateView(fetched_scan, leftFromViewfinder, rightFromViewfinder, frontFromViewfinder);

		if ((leftFromViewfinder > threshold) && (rightFromViewfinder > threshold))
		{ // both sides are open, so move forward and then guess

			// motion forward 
			do{
				viewFinder.updateView(fetched_scan, leftFromViewfinder, rightFromViewfinder, frontFromViewfinder);
			} while (frontFromViewfinder > threshold); // threshold for front should probably be the same value

			// guess
		}
		else if (leftFromViewfinder > threshold)
		{
			// motion forward .				********************* might need to change to a while loop *********************
			do{
				viewFinder.updateView(fetched_scan, leftFromViewfinder, rightFromViewfinder, frontFromViewfinder);
			} while (frontFromViewfinder > threshold); // threshold for front should probably be the same value

			// Turn left
		}
		else if (rightFromViewfinder > threshold)
		{
			// motion forward 
			do{
				viewFinder.updateView(fetched_scan, leftFromViewfinder, rightFromViewfinder, frontFromViewfinder);
			} while (frontFromViewfinder > threshold); // threshold for front should probably be the same value

			// Turn right
		}
	}
}