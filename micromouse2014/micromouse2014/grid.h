//
//  grid.h
//  micromouse2014
//
// Created by Carlton Semple, Lyle Moffitt, & Nick Beni
// Created at Syracuse University for the 2014 micromouse competition
//

#ifndef __micromouse2014__grid__
#define __micromouse2014__grid__


#include <iostream>
#include <vector>



#include "wall.h"
#include "cell.h"

namespace grid_config 
{
    /// The size of the maze in cells
    const int MAZE_SIZE = 16;
    

	const int mazeSize;
	double goalX, goalY;
	int  startingRow =0, startingColumn=0;
    

}



class grid
{
	friend class star;
	
    // A pair of axis intercepts for each cell element
    // The intercept is a line of cell references
	// It should not be assumed that these are cartesian coordinates
	std::vector<cell*> lat_headers; // rows
	std::vector<cell*> long_headers; // columns
	
	cell	*	origin;
	cell	*	goal;
	cell	*	center;
public:
	grid();
	grid(uint x_dim, uint y_dim);
    
    cell *getCell(double row, double column);
    cell *findCell(double x, double y);
    void addCell(cell &newcell);
    void createMaze(); // create the linked lists of cells, 16 by 16
	void markGoalCells();
    
    int  updateMaze(_360_scan wall_points);
	void wallOrienter(wall &wallInQuestion, 
                      std::string &orientation, 
                      double &x_displacement, 
                      double &y_displacement, 
                      double distanceToWall     );
	void addBasedOnCompass(wall &wallInQuestion, 
                           std::string wallOrientation, 
                           double &x_displacement, 
                           double &y_displacement, 
                           double distanceToWall    );
	void markWalls(wall &wallInQuestion, 
                   double &staticCoord, 
                   double &coordinateAlongWall, 
                   std::string &xORy        );
	cell *getPointerToJunction(char &sourceDirection);
	cell *findClosestGoalCell(double x, double y);
	bool closeEnough(double angle1, double angle2);

	double mazeSize;
	double cellsize;
	double compass;
	double xDistance, yDistance;
	double goalX, goalY;

	// probably not needed
	string direction;
	double shift;
	int startingRow;
	int startingColumn;

	
};

#endif /* defined(__micromouse2014__grid__) */
