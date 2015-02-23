#include "Searches.h"
#include "Sorts.h"

//#include "Cell_new.h"


namespace Algorithm
{
	/**
	* Update the cells' heuristic values starting from the given origin cell
	*/
	void Searches::breadth_first_search(Maze &maze, int x, int y)
	{
		std::queue<Room*> * q = new std::queue<Room*>();	// queue holds child cells

		//std::array<std::array<Room, 16>, 16> & maze = mazeClass.maze;
		//auto &maze = mazeClass.maze;
		Room * current_room = maze(x, y);

		current_room->set_breadth_heuristic(0);	// start heuristic count at 0

		int heuristic = 1, hCountDown = 0; // heuristic and variable to control when the heuristic increases

		int x_current = x, y_current = y;

		do
		{
			// add children to queue
			std::vector<Room*> children;

			Room * test = maze(x_current - 1, y_current);

			//*************************************         fix

			// Children should be the reachable ones

			// get children and not the current "parent"
			if (x_current - 1 > 0)	// left
				children.push_back(maze(x_current - 1, y_current));
			if (y_current + 1 < 16)	// top
				children.push_back(maze(x_current, y_current + 1));
			if (x_current + 1 < 16)	// right
				children.push_back(maze(x_current + 1, y_current));
			if (y_current - 1 > 0)	// bottom
				children.push_back(maze(x_current, y_current - 1));

			std::vector<Room*>::iterator cIt = children.begin();
			while (cIt != children.end())
			{
				// Do each node only once
				if ((*cIt)->get_breadth_heuristic() == std::numeric_limits<int>::max())
				{
					(*cIt)->set_breadth_heuristic(current_room->get_breadth_heuristic() + 1);   // set heuristic to parent's heuristic + 1
					q->push((*cIt));                              // add to queue
				}
				++cIt;
			}
			delete(&children);

			current_room = q->front();		// move to next child
			q->pop();
		} while (q->size() > 0);
	}


	std::deque<Path_new*> & Searches::depth_search(Maze &maze, Location &start, Location &goal)
	{
		std::deque<Path_new*> paths;
		Path_new * current_path = new Path_new();

		// Temporarily hold rooms and the paths to each 
		std::stack<Room*> * stac = new std::stack<Room*>();
		std::stack<Path_new*> * pstack = new std::stack<Path_new*>();

		// clear the checked values of all the cells (set to false)
		
		Room * current_room = maze(start.x, start.y);

		do
		{
			// get reachable child cells
			std::vector<Room*> * children = current_room->get_children();
			// Sort the children according to their weight (greatest is first)
			Sorting::Sorts<Room*>::quicksort(*children, 0, children->size());
			Sorting::Sorts<Room*>::reverse(*children);

			// Add children to stack
			while (children->size() > 0)
			{
				std::vector<Room*>::reverse_iterator nIt = children->rbegin();
				stac->push(*nIt);	// Push the child with the least weight/confidence first
				pstack->push(new Path_new(current_path, *nIt));

				children->pop_back();
			}

			// Add path to list of paths
			paths.push_back(current_path);

			// Look at next room & path
			current_room = stac->top();
			current_path = pstack->top();
			stac->pop();
			pstack->pop();

		} while (stac->size() > 0);
		

		return paths;
	}
}