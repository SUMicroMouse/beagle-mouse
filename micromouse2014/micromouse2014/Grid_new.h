#ifndef __GRID_NEW_H__
#define __GRID_NEW_H__

#include "Cell_new.h"
#include <iostream>
#include <vector>

/**
* Grid 
*/
class Grid_new
{
public:
	Grid_new();
	Grid_new(int sizeX, int sizeY);
	~Grid_new();

	void update_current_cell(Cell_new * cell);

	int size(){ return cell_count; }

	void print();				// debugging use

private:
	// cells for viewing the grid externally
	Cell_new * current_cell;
	Cell_new * previous_cell;

	Cell_new * zero_zero;		// pointer to cell at start of grid
	Cell_new * x_y;				// pointer to cell at last position of grid sizeX, sizeY

	int x_count;
	int y_count;
	int cell_count;
};

Grid_new::Grid_new()
{
}

Grid_new::Grid_new(int sizeX, int sizeY)
{
	x_count = sizeX;	y_count = sizeY;
	cell_count = sizeX * sizeY;

	// Must be at least a 2x2 grid
	if (cell_count < 4)
		return;

	// Use a vector to assist in making a 4-way linked list
	std::vector<Cell_new*> column;
	zero_zero = new Cell_new(0, 0);						// Set up first cell
	column.push_back(zero_zero);

	// Add & link the rest of the cells in the 1st column
	for (int i = 1; i < sizeY; i++)
	{
		Cell_new * previous = column.back();			// get last item in vector
		Cell_new *ncell = new Cell_new(0, i);			// first column: x = 0

		// Connect the nodes before adding to the vector
		previous->top = ncell;
		previous->top_const = ncell;
		ncell->bottom = previous;
		ncell->bottom_const = previous;

		column.push_back(ncell);						// add to vector
	}


	// Add the rest of the columns
	for (int x = 1; x < sizeX; x++)
	{
		// Shift the whole vector to the next column
		int y = 0;
		for (int cc = 0; cc < column.size(); cc++)
		{
			Cell_new * cell = column[cc];
			Cell_new *right = new Cell_new(x, y);
			cell->right = right;
			cell->right_const = cell->right;
			right->left = cell;
			right->left_const = cell;

			column[cc] = cell->right;							// shift cell right
			y++;
		}

		// Link the cells in the column
		std::vector<Cell_new*>::iterator p = column.begin(), pAhead = column.begin();
		pAhead++;
		while (pAhead != column.end())
		{
			auto prev_cell = *p;	auto ce = *pAhead;
			// link cells
			prev_cell->top = ce;
			prev_cell->top_const = ce;
			ce->bottom = prev_cell;
			ce->bottom_const = prev_cell;

			++p;	++pAhead;
		}
		
		// set up end pointer
		if (x == sizeX - 1)
			x_y = *p;
	}

}

Grid_new::~Grid_new()
{
}

void Grid_new::update_current_cell(Cell_new * cell)
{
	previous_cell = current_cell;
	current_cell = cell;
}

/**
* Print grid in reverse, so that it appears with typical cartesian coordinates
*/
void Grid_new::print()
{
	auto row_start = x_y;				// start at end
	while (row_start->left != nullptr)
		row_start = row_start->left;	// get to y=0 column

	while (row_start != nullptr)
	{
		// iterate through the row
		Cell_new *iterator = row_start;
		while (iterator != nullptr)
		{
			iterator->print();
			std::cout << "   ";
			iterator = iterator->right;
		}
		std::cout << std::endl;			// new line
		row_start = row_start->bottom;	// next row down
	}
}

#endif