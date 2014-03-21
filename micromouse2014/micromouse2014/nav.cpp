#include <iostream>
#include "nav.h"
#include "motor.h"

using namespace std;

// go forward in a small enough chunk (one cell approx.) so that the distance to the opposing wall can be scanned/updated
void nav::goForwardOne()
{
    
}



// turn by so many degrees, determined by time
// assuming the front middle is 0 degrees
void star::turn(double angle)
{
	double middle; // just here temporarily
	double degrees = middle - angle;
	
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


void nav::curveleft(float VR, float speed)
{
	VL = 5; 



};

void nav::turnleft(float VR, float VL)
{

	VR = 5;
	VL = -5;

};

void nav::turnright(float VR, float VL)
{

	VR = -5;
	VL =  5;

};


void nav::moveforward(float VR, float VL)
{
	
	VR = 5;
	VL = 5;


};

void nav::movebackward(float VR, float VL)
{

	VR = -5;
	VL = -5;



};

void nav::coast(float VR, float VL)
{


	VR = VR/2;
	VL = VL/2;




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

void nav::stop(float VR, float VL)
{

	VR = 0;
	VL = 0;


};