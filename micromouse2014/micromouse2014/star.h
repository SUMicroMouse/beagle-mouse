//
//  star.h
//  micromouse2014
//
//
// Created by Carlton Semple, Lyle Moffitt, & Nick Beni
// Created at Syracuse University for the 2014 micromouse competition

#ifndef __micromouse2014__star__
#define __micromouse2014__star__


#include <iostream>
#include <vector>
#include <deque>
#include <stack>
#include <string>


#include "config.h"
#include "grid.h"


typedef
std::vector<cell*> path;

typedef
std::vector<packet*> packets;

typedef
std::deque<wall*> walls;

typedef
std::vector<double> vect;

typedef
std::string string;


// contains 
class item{

};

/* This will simplify data input into 19 regions. 
The angle of the data from the lidar will determine the position in viewfinder, 
	and the distance/radius will be used to determine when an open space is coming up on the left/right, or a wall ahead.
The number of items in the viewfinder may be increased for better accuracy and increased response time*/
class local_grid{
	std::vector<double> viewfinder;
	std::vector<double> previous;
public:
	local_grid() { viewfinder.resize(90); previous.resize(90); }

	// takes the grid from the lidar scan and simplifies the data
	void updateView(grid &fetched_scan, int &returnedLeft, int &returnedRight, double &returnedFront); 
};



class star
{
	// size of cells
	double lengthwidth;
	double threshold;	// distance at which it's determined the side walls aren't there
	double front_threshold; // about the length of a cell. 
							// when it's determined that an open side is on the left/right, this says if there is a wall in front or not
	grid maze;	
	/*path to_orig;
	path guess;*/
	string direction; // direction that mouse is facing
	double compass;
	int rightTurns;
	int leftTurns;
	std::vector<cell *> traversed;
	// old
	///*********** Local Grid ************/
	//int leftFromViewfinder; // will get the region that the open space (to the left) is currently at in relation to the lidar
	//int rightFromViewfinder; // will get the region that the open space (to the right) is currently at in relation to the lidar
	//double frontFromViewfinder; // gets the distance of the region directly in front

	/************************* new ********************/
	double xDistance, yDistance; // the current position of the robot
	double shift; // represents the difference between the current compass heading and the default, which is 90 degrees
	std::vector<packet*> vision;
	std::deque<wall*> wallDeq;
	int num_packets; // 90
	std::vector<cell*> lat_headers; // rows
	std::vector<cell*> long_headers; // columns
	int mazeSize;
	double goalX;
	double goalY;
	int startingRow, startingColumn;
	double headOnDistance;
	double headOnDistance2;

	cell * getPointerToJunction(char &sourceDirection);
	bool atJunction;
public:
	star();


	// starting fresh
	void scan(); // modifies local grid
	void turn(double angle);
	void createMaze(); // create the linked lists of cells, 16 by 16
	void markGoalCells();
	void addCell(cell &newcell);
	cell *findCell(double x, double y);
	cell *getCell(double row, double column);
	int updateMaze();
	void wallOrienter(wall &wallInQuestion, string &orientation, double &x_displacement, double &y_displacement, double distanceToWall);
	void addBasedOnCompass(wall &wallInQuestion, string wallOrientation, double &x_displacement, double &y_displacement, double distanceToWall);
	void markWalls(wall &wallInQuestion, double &staticCoord, double &coordinateAlongWall, string &xORy);
	//void opposingWall();

	// algorithm, decision-making
	int decide();
	void determineMovementCost(cell &ce);
	void determineheuristicCost();
	void goForwardOne();
	void PositionChange();

	// determining movementCost & heuristicCost
	void breadthSearch();
	void pushChildCellsToDeque(std::deque<cell*> &childCells);
};

#endif /* defined(__micromouse2014__star__) */
