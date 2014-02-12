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

	void	turn_left(  	unsigned degrees);
	void	turn_right(  	unsigned degrees);
	void	move_forward(	unsigned distance);
	void	move_backward(	unsigned distance);
	
public:
	void	face_left(){	turn_left(90); }
	void	face_right(){	turn_right(90); }
	void	advance(){  	move_forward(CELL_SIZE); }
};


#endif /* defined(__micromouse2014__robot__) */
