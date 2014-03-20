//
//  robot.cpp
//  micromouse2014
//
//  Created by Lyle Moffitt on 1/21/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#include "robot.h"

void 
robot::turn_left(unsigned proxydegrees) // unsigned degrees may not work, using proxy variables
{
    While(encoder_value < whatever_ammount)  // whatever_ammount is the right encoder value to make a 90 deg turn (to the left)
    {
        bool Left_Motor = false;
        bool Right_Motor = true;
    }
}

void	
robot::turn_right(  	unsigned proxydegrees)
{
    while(encoder_value < whatever_ammount) // see turn_left comment
    {
        bool Left_Motor = true;
        bool Right_Motor = false;
    }
}

void 
robot::move_forward( unsigned proxydegrees) //dead reckoning style 
{
    signed int voltage = 5;
    bool Left_Motor = true;
    bool Right_Motor = true;
}

void 
robot::move_backward( unsigned proxydegrees)
{
    signed int voltage = -5;
    bool Left_Motor = true;
    bool Right_Motor = true;
}