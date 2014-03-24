//
//  nav.cpp
//
// Created by Carlton Semple, Lyle Moffitt, & Nick Beni
// Created at Syracuse University for the 2014 micromouse competition
//


#include "nav.h"




using namespace std;

// Defualt constructor
nav::nav(const lidar * lidar_ptr):
view(lidar_ptr),
left(motor_config::m_select::LEFT),
right(motor_config::m_select::RIGHT)
{
    
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
nav::curveleft(float VL ,float speed)
{
	
}

void 
nav::curveright(float VR, float speed)
{
	
}

void 
nav::turnleft(double angle)
{
    
	if(angle >= 90)
	{
	left.set_speed(-angle);
	right.set_speed(angle);
	}
	else if(angle <= 90)
	{
	turnright(angle);
	}
    
}

void 
nav::turnright(double angle)
{
    
	if(angle <= 90)
	{
	left.set_speed(angle);
	right.set_speed(-angle);
	}
	else if(angle >= 90)
	{
	turnleft(angle);
	}
    
}


void nav::moveforward()
{
    double speed = min_speed;
	left.enable();
	right.enable();
	
	synchronize(speed);
	
	left.set_speed(speed);
	right.set_speed(speed);

	left.get_speed();
	right.get_speed(); // 	 

	while(left.get_speed() == 1 || right.get_speed() == 1)
	synchronize(speed);
	 
}

void nav::movebackward()
{
	double speed = min_negspeed;
	left.enable();
	right.enable();

	synchronize(speed);

	left.set_speed(speed);
	right.set_speed(speed);

	left.get_speed();
	right.get_speed();
    
	while(left.get_speed() == 0 || right.get_speed() == 0)
	synchronize(speed);

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

void 
nav::XLR8()
{
    left.enable();
	right.enable();

	synchronize();

	do
	{
	right.get_speed();
	left.get_speed();
	}
	while(right.get_speed() && left.get_speed() != 0);
	nav::moveforward();

}

void 
nav::coast() //not sure if neccessary  // let's try toggling the state of the motors to slow it down
{
    left.enable();
    right.enable();
	left.disable();
	right.disable();
    
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

void 
nav::movedistancevariable(int dist)
{
	for(int i= 0;i<dist;i++)
	goForwardOne();
    
}

int 
nav::getdistance_forward() // fetches the distance forward
{	
    
	int dist;
	
    do 
	{ 
        dist = lidar.last_scan()->deg_index.at(lidar_config:: degree_north).second()->eval_dist();
	}
	while(dist == -1);
    
	
    
	return dist;
}






