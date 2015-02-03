#ifndef __GRID_NEW_H__
#define __GRID_NEW_H__

#include "Cell_new.h"

class Grid_new
{
public:
	Grid_new();
	Grid_new(int sizeX, int sizeY);
	~Grid_new();

	void update_current_cell(Cell_new * cell);

private:
	Cell_new * current_cell;
	Cell_new * previous_cell;
};

Grid_new::Grid_new()
{
}

Grid_new::Grid_new(int sizeX, int sizeY)
{

}

Grid_new::~Grid_new()
{
}

void Grid_new::update_current_cell(Cell_new * cell)
{
	previous_cell = current_cell;
	current_cell = cell;
}

#endif