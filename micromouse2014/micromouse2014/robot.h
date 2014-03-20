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


#endif /* defined(__micromouse2014__robot__) */
