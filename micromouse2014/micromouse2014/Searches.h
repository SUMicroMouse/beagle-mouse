#ifndef __SEARCHES_H__
#define __SEARCHES_H__

#define NOMINMAX

#include <queue>
#include <vector>
#include "Cell_new.h"
#include "Maze.h"

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
		static void breadth_first_search(Maze &maze, int x, int y);

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

	/**
	* Update the cells' heuristic values starting from the given origin cell
	*/
	void Searches::breadth_first_search(Maze &mazeClass, int x, int y)
	{
		std::queue<Room*> * q = new std::queue<Room*>();	// queue holds child cells

		auto maze = mazeClass.maze;
		auto current_room = maze[y][x];

		current_room.set_breadth_heuristic(0);	// start heuristic count at 0

		int heuristic = 1, hCountDown = 0; // heuristic and variable to control when the heuristic increases

		int x_current = x, y_current = y;
		
		do
		{

			// add children to queue
			std::vector<Room> children; 

			// get children and not the current "parent"
			if (x_current - 1 > 0)	// left
				children.push_back(maze[y_current][x_current - 1]);
			if (y_current + 1 < maze.size())
				children.push_back(maze[y_current + 1][x_current]);

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

			current_cell = q->front();		// move to next child
			q->pop();
		} while (q->size() > 0);
	}

}

#endif