//
//  star.h
//  micromouse2014
//
//  Created by Lyle Moffitt on 1/21/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#ifndef __micromouse2014__star__
#define __micromouse2014__star__

#include <iostream>
#include <stack>


#include "grid.h"
#include "config.h"

typedef
std::stack<grid &> path;

class star
{
	grid	maze;
	
	path	to_orig;
	path	guess;
	
public:
	star();
	
	
};

#endif /* defined(__micromouse2014__star__) */
