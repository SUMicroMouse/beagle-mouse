//
//  robot.h
//  micromouse2014
//
//  Created by Lyle Moffitt on 3/20/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#ifndef __micromouse2014__robot__
#define __micromouse2014__robot__

#include <iostream>


#include "lidar.h"
#include "motor.h"

class robot
{
    std::string direction; // direction that mouse is facing
	double compass;
    double xDistance, yDistance; // the current position of the robot
	double shift; // represents the difference between the current compass heading and the default, which is 90 degrees

public:
    void PositionChange();
    
};


#endif /* defined(__micromouse2014__robot__) */
