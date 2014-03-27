//
//  nav.h
//
// Created by Carlton Semple, Lyle Moffitt, & Nick Beni
// Created at Syracuse University for the 2014 micromouse competition
//

#include "motor.h"
#include "encoder.h"

#include "lidar.h"
#include "packet.h"

#include <iostream>


namespace nav_config
{
	constexpr double max_speed = 1; 
	constexpr double min_speed = 0.5;
	constexpr double min_negspeed = 0.5;
	constexpr double max_negspeed = 0;
	constexpr double max_invariance = 0.0001;
	constexpr double min_invariance = 0.00000001;
	constexpr size_t duration = 10000000;


}
//change

class nav
{
private:
	motor left; 
	motor right;
    
    encoder enc;
    
    lidar& view;

public:
    
    // Class constructor for navigation
    nav(lidar & lidar_ptr);

	//makes sure wheels are moving in step at same speed 
	 void synchronize(double speed);
	 
	// assuming the front middle is 0 degrees
	void turn(double angle);

	// go forward in a small enough chunk (one cell approx.) so that the distance to the opposing wall can be scanned/updated
	void goForwardOne();
	
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
	
	//right motor maintains constant speed; left adjusts with ratio to 
	void veerright(double ratio);
	
	//accelerates the rover forward  
	void XLR8();
	
	//cuts speed down? 
	void coast();
	
	//stops motor 
	void stop();
	
	//moves a distance in terms of mm 
	void movedistancevariable(float VL, float VR);
	
	int getdistance_forward(); // fetches the distance forward
	
};

