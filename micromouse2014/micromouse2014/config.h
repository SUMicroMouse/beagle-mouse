//
//  config.h
//  micromouse2014
//
// Created by Carlton Semple, Lyle Moffitt, & Nick Beni
// Created at Syracuse University for the 2014 micromouse competition
//

/** This is a header file for centralizing all configuration options.
 */

#ifndef micromouse2014_config_h
#define micromouse2014_config_h



/*****************************************************************************/
/** Maze presets.
 *	These are used to set various configuration options about the maze
 */
/// The size of the maze in cells
#define MAZE_SIZE 16

/// The length of the cell in inches
#define CELL_SIZE 8


/*****************************************************************************/
/** Lidar options
 */
/// The number of roations per second
#define LIDAR_RPM	10

/// The depth of the lidar scan in cells
#define LIDAR_DEPTH 2


/*****************************************************************************/
/** Debugging options.
 *	You can use these macros to turn on and off various lines of code
 *	for example, placing DL2 before the function call like so:
 *		DL2 function();
 *	will only call the function if the second place of DBG_SET is less than 1.
 *	Otherwise, everything up to the first semicolon will be prevented from
 *  executing.
 */

/// Used to turn various debugging options on and off
#define DBG_SET 00000000
//              87654321

// Defines debugging level 1
#define LVL 1
#if( DBG_SET % (10^LVL) > (10^(LVL-1)) )
#define DL1 if(1)
#else
#define DL1 if(0)
#endif
#undef LVL

// Defines debugging level 2
#define LVL 2
#if( DBG_SET % (10^LVL) > (10^(LVL-1)) )
#define DL2 if(1)
#else 
#define DL2 if(0)
#endif
#undef LVL



#undef DBG_SET

#endif //define micromouse2014_config_h
