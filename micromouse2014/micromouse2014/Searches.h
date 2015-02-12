#ifndef __SEARCHES_H__
#define __SEARCHES_H__

#include <queue>
#include "Cell_new.h"

namespace Algorithm
{
	/**
	* Class that contains a breadth-first search, which updates cells' heuristic values
	*/
	class Searches
	{
	public:
		// Update the cells' heuristic values starting from the given cell
		static void breadth_first_search(Cell_new * origin);

	private:
		// Probably will need a variable to represent when a search is in progress vs. finished
		// so that multithreading works properly.. "condition variable"
	};	

	/**
	* Update the cells' heuristic values starting from the given cell
	*/
	void Searches::breadth_first_search(Cell_new * origin)
	{
		std::queue<Cell_new*> * q = new std::queue<Cell_new*>();	// queue holds child cells

		Cell_new * current_cell = origin;
		
		int heuristic = 0;

		do
		{
															// add children to queue
			std::vector<Cell_new*> * children = current_cell->children();
			std::vector<Cell_new*>::iterator cIt = children->begin();
			while (cIt != children->end())
			{
				(*cIt)->set_parent(current_cell);			// set parent pointers for the child
				(*cIt)->set_breadth_heuristic(heuristic);	// set heuristic
				
				q->push(*cIt);								// add to queue
				++cIt;
			}

			current_cell = q->front();						// move to next child
			q->pop();
			heuristic++;									// increase heuristic
		} while (q->size() > 0);
	}
}

#endif