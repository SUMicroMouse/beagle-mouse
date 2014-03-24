#include <iostream>
#include "nav.h"




using namespace std;

// go forward in a small enough chunk (one cell approx.) so that the distance to the opposing wall can be scanned/updated
void nav::goForwardOne()
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



};



// turn by so many degrees, determined by time
// assuming the front middle is 0 degrees
void nav::turn(double angle)
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
};


void nav::curveleft(float VR, float speed)
{
	



};

void nav::turnleft(double angle)
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

};

void nav::turnright(double angle)
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


};


void nav::moveforward()
{
	
	left.enable();
	right.enable();
	
	synchronize();
	
	left.set_speed();
	right.set_speed();

	left.get_speed();
	right.get_speed(); // 

	while(left.get_speed() == 1 || right.get_speed() == 1)
	synchronize();
	 
};

void nav::movebackward()
{

	left.enable();
	right.enable();

	synchronize();

	left.set_speed();
	right.set_speed();

	left.get_speed();
	right.get_speed();
	
	while(left.get_speed() == 0 || right.get_speed() == 0)
	synchronize();

};

void nav::coast()
{






};

void nav::XLR8(float VR, float VL)
{

	VR = VR*2;
	VL = VL*2;


};


void nav::veerleft(float VR, float VL)
{


	


};


void nav::veerright(float VR, float VL)
{





};

void nav::stop()
{
	
	left.disable();
	right.disable();
	
};

void nav::synchronize(double speed) // should go in go forward
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
};

void movedistancevariable(float VL, float VR)
{
	lidar.last;




};

int getdistance_forward() // fetches the distance forward
{	

	int dist;
	
do 
	{ 
	dist = lidar.last_scan()->deg_index.at(lidar_config::degree_north).second()->eval_dist();
	}
	while(dist = -1);
		
	

	return dist;
};