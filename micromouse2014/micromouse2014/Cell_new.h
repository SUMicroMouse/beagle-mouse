#ifndef __CELL_NEW_H__
#define __CELL_NEW_H__

#include "Coordinate_new.h"
#include <iostream>

/**
* The cell class is contained by the Grid class.  A cell contains publicly accessible
* pointers to reachable adjacent cells.  There also exists hidden points to adjacent cells,
* which always exist only for the Grid class to access.
* Adjacent cells are to be set to nullptr once it's determined that there's a wall between them
*/
class Cell_new
{
public:
	Cell_new();
	Cell_new(int x, int y);
	~Cell_new();

	// Public coordinate getter
	Coordinate_new * get_coordinate(){ return coordinate; }
	
	// Adjacent cell getters
	Cell_new * left_wall();
	Cell_new * top_wall();
	Cell_new * right_wall();
	Cell_new * bottom_wall();

	// Adjacent cell setters
	void left_wall(Cell_new * cell);
	void top_wall(Cell_new * cell);
	void right_wall(Cell_new * cell);
	void bottom_wall(Cell_new * cell);

	void print();				// debugging use

private:
	friend class Grid_new;		// Give Grid class special access

	Coordinate_new *coordinate;	

	// Adjacent cell pointers that don't change
	Cell_new * left_const;
	Cell_new * top_const;
	Cell_new * right_const;
	Cell_new * bottom_const;

	// Publicly accessible adjacent cell pointers that can change
	Cell_new * left;
	Cell_new * top;
	Cell_new * right;
	Cell_new * bottom;
};

Cell_new::Cell_new()
{
	coordinate = new Coordinate_new();
}

Cell_new::Cell_new(int x, int y)
{
	coordinate = new Coordinate_new(x, y);
}

Cell_new::~Cell_new()
{
	delete(coordinate);
}

/**
* Print the cell's walls if they exist, and the cell's coordinates
*/
void Cell_new::print()
{
	if (this->left == nullptr)		// left wall
		std::cout << "|";
	else
		std::cout << " ";
	if (this->top == nullptr)		// top wall
		std::cout << "*";
	else
		std::cout << " ";
	if (this->coordinate != nullptr)
	{
		std::cout << " ";
		this->coordinate->print();	// coordinate
		std::cout << " ";
	}
	if (this->bottom == nullptr)	// bottom wall
		std::cout << "_";
	else
		std::cout << " ";
	if (this->right == nullptr)		// right wall
		std::cout << "|";
	else
		std::cout << " ";
}

#endif