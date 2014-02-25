//
//  robot.h
//  micromouse2014
//
// Created by Carlton Semple, Lyle Moffitt, & Nick Beni
// Created at Syracuse University for the 2014 micromouse competition
//

#ifndef __micromouse2014__robot__
#define __micromouse2014__robot__

#include "config.h"


/** A class of methods to control the robot.
 *	These methods will be eventually filled in with the code that Nick writes.
 *	Only basic methods are needed for now. 
 */

class robot
{

	void	turn_left(  	unsigned proxydegrees);
	void	turn_right(  	unsigned proxydegrees);
	void	move_forward(	unsigned proxydistance);
	void	move_backward(	unsigned proxydistance);
	
public:
	void	face_left(){	turn_left(90); }  // not sure if neccessary turn left + move forward does same job
	void	face_right(){	turn_right(90); } // not sure if neccessary see comment above 
	void	advance(){  	move_forward(CELL_SIZE); }
    void    whatstheoppositeofadvance() { move_backward(CELL_SIZE);}
};

void turn_left(unsigned proxydegrees) // unsigned degrees may not work, using proxy variables
{
While(encoder_value < whatever_ammount)  // whatever_ammount is the right encoder value to make a 90 deg turn (to the left)
{
bool Left_Motor = FALSE;
bool Right_Motor = TRUE;
}
}

void	turn_right(  	unsigned proxydegrees)
{
while(encoder_value < whatever_ammount) // see turn_left comment
{
bool Left_Motor = TRUE;
bool Right_Motor = FALSE;
}
}

void move_forward( unsigned proxydegrees) //dead reckoning style 
{
signed int voltage = 5;
bool Left_Motor = TRUE;
bool Right_Motor = TRUE;
}

void move_backward( unsigned proxydegrees)
{
signed int voltage = -5;
bool Left_Motor = TRUE;
bool Right_Motor = TRUE;
}
#endif /* defined(__micromouse2014__robot__) */
