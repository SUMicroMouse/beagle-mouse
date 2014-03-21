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


#include "grid.h"
#include "wall.h"

#include "robot.h"

namespace star_config
{
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
}


/* This will simplify data input into 19 regions. 
The angle of the data from the lidar will determine the position in viewfinder, 
	and the distance/radius will be used to determine when an open space is coming up on the left/right, or a wall ahead.
The number of items in the viewfinder may be increased for better accuracy and increased response time*/
class local_grid
{
	std::vector<double> viewfinder;
	std::vector<double> previous;
    
public:
	local_grid(): viewfinder(90), previous(90){}

	// takes the grid from the lidar scan and simplifies the data
	void updateView(grid &fetched_scan,
                    int &returnedLeft, 
                    int &returnedRight,
                    double &returnedFront       ); 
};



class star
{
	grid maze;	
    
    robot rob;
    
	int rightTurns;
	int leftTurns;
	std::vector<cell *> traversed;

    std::vector<cell*> lat_headers; // rows
	std::vector<cell*> long_headers; // columns
	
    _360_scan vision;
	std::deque<wall*> wallDeq;
    
#define num_packets packet_config::index_range // 90

	double headOnDistance;
	double headOnDistance2;

	bool atJunction;
    
    
public:
	star();

	// starting fresh
	void scan(); // modifies local grid	
	
		//void opposingWall();

	// algorithm, decision-making
	int decide();
	void determineMovementCost(cell &ce);
	void determineheuristicCost();
	
	

	// determining movementCost & heuristicCost
	void breadthSearch();
	int depthSearch(cell &sender, cell &current, std::stack<cell*> &tempStack, std::deque<cell*> &path);
	void pushChildCellsToDeque(std::deque<cell*> &childCells);
};

#endif /* defined(__micromouse2014__star__) */
