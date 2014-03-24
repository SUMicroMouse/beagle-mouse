#include <iostream>
#include "motor.h"

using namespace std;

class nav
{
<<<<<<< HEAD
private:
	motor left; 
	motor right;
=======
    

>>>>>>> 584141a692497396a4241499915ffccd4f264129
public:
	//breaks the right motor, left motor moves forward 
	void curveleft(float VR, float speed);
	
	//breaks the left motor, right motor moves forward 
	void curveright(float VL);
	
	// turns rover 90 deg to the left (static)
	void turnleft(float VR, float VL);
	
	// turns rover 90 deg to the right  (static)
	void turnright(float VL, float VR);
	
	//moves the rover forward 
	void moveforward(float VL, float VR);
	
	//moves the rover backwards
	void movebackward(float VL,float VR);
	
	//spins the rover around 180 deg
	void turnaround(float VR, float VL);
	
	//veers the rover to the left 
	void veerleft(float VL, float VR);
	
	//veers the rover to the right 
	void veerright(float VL, float VR);
	
	//accelerates the rover forward  
	void XLR8(float VL, float VR);
	
	//cuts speed down? 
	void coast(float VL, float VR);
	
	//stops motor 
	void stop(float VL, float VR);
	
	//moves a distance in terms of mm 
	void movedistancevariable(float VL, float VR);
	
};

