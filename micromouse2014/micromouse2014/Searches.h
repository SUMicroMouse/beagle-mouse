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
	* Update the cells' heuristic values starting from the given origin cell
	*/
	void Searches::breadth_first_search(Cell_new * origin)
	{
		std::queue<Cell_new*> * q = new std::queue<Cell_new*>();	// queue holds child cells

		Cell_new * current_cell = origin;
		
		current_cell->set_breadth_heuristic(0);	// start heuristic count at 0

		int heuristic = 1, hCountDown = 0; // heuristic and variable to control when the heuristic increases

		do
		{
			if (current_cell != nullptr)
			{
				// add children to queue
				std::vector<Cell_new*> * children = current_cell->children(); // get children and not the current "parent"
				std::vector<Cell_new*>::iterator cIt = children->begin();
				while (cIt != children->end())
				{
					if ((*cIt) == nullptr)
					{
						cIt++;
						continue;
					}
					
					// Do each node only once
					if ((*cIt)->get_breadth_heuristic() == std::numeric_limits<int>::max())
					{
						(*cIt)->set_parent(current_cell);           // set parent pointer for the child
						(*cIt)->set_breadth_heuristic(current_cell->get_breadth_heuristic() + 1);   // set heuristic to parent's heuristic + 1
						q->push(*cIt);                              // add to queue
					}
					++cIt;
				}
				delete(children);
			}
			current_cell = q->front();		// move to next child
			q->pop();               	
		} while (q->size() > 0);
	}
}

#endif