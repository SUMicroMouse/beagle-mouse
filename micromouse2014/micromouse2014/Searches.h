#ifndef __SEARCHES_H__
#define __SEARCHES_H__

//#define NOMINMAX
#undef min
#undef max

#include <queue>
#include <vector>
#include <stack>
//#include "Cell_new.h"
#include "Maze.h"
#include "Path_new.h"

//#include "Searches.cpp"	// to get certain code to work inn the Searches.cpp file

//using namespace Algorithm;

namespace Algorithm
{
	/**
	* Class that contains a breadth-first search, which updates cells' heuristic values
	*/
	class Searches
	{
	public:
		// Update the cells' heuristic values starting from the given cell
		//static void breadth_first_search(Cell_new * origin);

		// Update the cells' heuristic values starting from the given cell
		static void breadth_first_search(Maze &mazeClass, int x, int y);

		Path_new & depth_search(Maze &mazeClass, int x, int y);

	private:
		// Probably will need a variable to represent when a search is in progress vs. finished
		// so that multithreading works properly.. "condition variable"
	};	
		
	
	


	/*Path_new & Searches::depth_search(Maze &mazeClass, int x, int y)
	{
		Path_new path;

		auto current_cell = mazeClass;

		return path;
	}*/
}

#endif