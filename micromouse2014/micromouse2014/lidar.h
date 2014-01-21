//
//  lidar.h
//  micromouse2014
//
//  Created by Lyle Moffitt on 1/21/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#ifndef __micromouse2014__lidar__
#define __micromouse2014__lidar__

#include <iostream>

#include "grid.h"

#include "config.h"

#define timestamp long unsigned

class lidar
{
	static const
	unsigned depth = LIDAR_DEPTH;
	
	/** Hold the most recent scan.
	 *	A scan is a radial grid.
	 *	Sample coordinates for cells.
	                 (3,2)
	 
	 
		     (2,2)   (2,1)   (4,2)
	 
	 
	 (1,2)   (1,1)   (0,0)   (3,1)   (5,2)
	 */



	
	
	grid & scan;
	
	grid & temp_scan;
	
	bool	is_old;
	
	timestamp last_updated;
	
	 
public:
	lidar();
	
	/** Scans the field in front are returns a *radial* grid of cells.
	 *	1)	check that polling() == false
	 *	2)	check that current() == false
	 */
	void update_scan();
	
	/// Returns the most recent scan
	grid &	fetch_scan();
	
	/// Returns true while there is no new scan to get.
	bool	current();
	
	/// Returns true if the grid is unfinished
	bool	polling();
	
};


#endif /* defined(__micromouse2014__lidar__) */
