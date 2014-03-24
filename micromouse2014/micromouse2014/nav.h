//
//  nav.h
//
// Created by Carlton Semple, Lyle Moffitt, & Nick Beni
// Created at Syracuse University for the 2014 micromouse competition
//

#include "motor.cpp"

#include "lidar.h"
#include "packet.h"

#include <iostream>


namespace nav_config
{
	const double max_speed = 1; 
	const double min_speed = 0.5;
	const double min_negspeed = 0.5;
	const double max_negspeed = 0;
	const double max_invariance = 0.0001;
	const double min_invariance = 0.00000001;
	const size_t duration = 10000000;
	


}
//change

class nav
{
private:
	motor left; 
	motor right;
    
    const 
    lidar* view;

public:
    // Class constructor for navigation
    nav(const lidar * lidar_ptr);
    

	//checks to see if wheels are in the same posistion 
	 void synchronize(double speed);
	 
	// assuming the front middle is 0 degrees
	void turn(double angle);

	// go forward in a small enough chunk (one cell approx.) so that the distance to the opposing wall can be scanned/updated
	void goForwardOne();

	//breaks the right motor, left motor moves forward 
	void curveleft();
	
	//breaks the left motor, right motor moves forward 
	void curveright();
	
	// turns rover 90 deg to the left (static)
	void turnleft(double angle);
	
	// turns rover 90 deg to the right  (static)
	void turnright(double angle);
	
	//moves the rover forward 
	void moveforward();
	
	//moves the rover backwards
	void movebackward();
	
	//spins the rover around 180 deg
	void turnaround(float VR, float VL);
	
	//veers the rover to the left 
	void veerleft(float VL, float VR);
	
	//veers the rover to the right 
	void veerright(float VL, float VR);
	
	//accelerates the rover forward  
	void XLR8();
	
	//cuts speed down? 
	void coast();
	
	//stops motor 
	void stop();
	
	//moves a distance in terms of mm 
	void movedistancevariable(int dist);
	
	int getdistance_forward(); // fetches the distance forward
	
};

