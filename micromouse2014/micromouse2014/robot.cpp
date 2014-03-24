//
//  robot.cpp
//  micromouse2014
//
//  Created by Lyle Moffitt on 3/20/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#include "robot.h"



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


cell * 
robot::getPointerToJunction(char &sourceDirection)
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
