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
    /* ____Primary methods:____ require specific arguments */
    
    // Class constructor for navigation
    nav(lidar & lidar_ptr);

    // moves a distance in terms of mm 
	void movedistancevariable(size_t mm);
    
	// makes sure wheels are moving in step at same speed 
	 void synchronize(double speed);
	 
	// assuming the front middle is 0 degrees; positive == clockwise 
	void turn(double angle);

    // move at constant velocity
    void move(double speed);
    
    /** veers the rover to the left based on ratio of speeds
     Positive:  right is increased relative to left
     Negative:  left is decreased relative to right
     */
	void veerleft(double ratio);
	
	/** veers the rover to the right based on ratio of speeds
     Positive:  left is increased relative to right
     Negative:  right is decreased relative to left
     */
	void veerright(double ratio);
    
    
    /* ____Secondary methods:____ call primaries with constant values */
    
	// go forward one unit cell and stop
	void goForwardOne();
	
	// turns rover 90 deg to the left (static)
    inline
	void turnleft(){    turn(-90);  }
	
	// turns rover 90 deg to the right  (static)
    inline
	void turnright(){   turn(90);   }
	
	//spins the rover around 180 deg
    inline
	void turnaround(){  turn(180);  }
    
	//stops motor 
	void stop();

	

		
};

