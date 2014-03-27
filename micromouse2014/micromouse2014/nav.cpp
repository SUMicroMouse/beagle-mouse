//
//  nav.cpp
//
// Created by Carlton Semple, Lyle Moffitt, & Nick Beni
// Created at Syracuse University for the 2014 micromouse competition
//


#include "nav.h"


using namespace std;

// Defualt constructor
nav::nav(lidar & lidar_ptr):
view(lidar_ptr),
left(motor_config::m_select::LEFT),
right(motor_config::m_select::RIGHT)
{
    
}


void 
nav::movedistancevariable(size_t mm)
{
    
}


void 
nav::synchronize(double speed) // should go in go forward
{
    
	double goal_speed_L = speed;
	double goal_speed_R = speed;
    
	if(left.chk_en() = false || right.enable() == false)
	{
		left.enable();
		right.enable();
	}
    
	double avg_speed = (left.get_speed() + right.get_speed())/2.0;
    
	if(goal_speed_L < goal_speed_R)
	{	goal_speed_L = max_invariance + goal_speed_L;}
    
	else(goal_speed_R < goal_speed_L)
	{goal_speed_R = max_invariance + goal_speed_R;}
    
	else(goal_speed_R == goal_speed_L)
    {goal_speed_L = avg_speed;
		goal_speed_R = avg.speed;}
}


// turn by so many degrees, determined by time
// assuming the front middle is 0 degrees
void 
nav::turn(double angle)
{
	double middle; // just here temporarily
	double degrees = middle - angle;
	double compass;
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

void
nav::move(double speed)
{
    
}

void 
nav::veerleft(float VR, float VL)
{
    moveforward();
	left.disable();
	XLR8();
	left.enable();
}


void 
nav::veerright(float VR, float VL)
{
    moveforward();
	right.disable();
	XLR8();
	right.enable();
}


// go forward in a small enough chunk (one cell approx.) so that the distance to the opposing wall can be scanned/updated
void 
nav::goForwardOne()
{
    double val; // give a specifc amount 
                //left motor 
                // right motor both need to advance so both are going in the same direction
	left.enable();
	right.enable();
    
    left.set_speed();
    right.set_speed();
    
    
    left.get_speed();
    right.get_speed();
    
    synchronize();
}


void 
nav::stop()
{
	while(left.chk_en() || right.chk_en() == 1)
	{
	left.disable();
	right.disable();
	}
}






