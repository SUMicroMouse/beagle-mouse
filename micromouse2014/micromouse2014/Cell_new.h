#ifndef __CELL_NEW_H__
#define __CELL_NEW_H__

#include "Coordinate_new.h"
#include "Direction.h"
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
	
	
	Cell_new *wall(Direction direction);					// Adjacent cell getter
	void set_wall(Direction direction, Cell_new * cell);	// Adjacent cell setter

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
* Return the specified cell
*/
Cell_new * Cell_new::wall(Direction direction)
{
	switch (direction)
	{
	case L: return left;
		break;
	case T: return top;
		break;
	case R: return right;
		break;
	case B:	return bottom;
		break;
	default:
		break;
	}
	return nullptr;
}

void Cell_new::set_wall(Direction direction, Cell_new * cell)
{
	switch (direction)
	{
	case L:	left = cell;
		break;
	case T: top = cell;
		break;
	case R: right = cell;
		break;
	case B: bottom = cell;
		break;
	default:
		break;
	}
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