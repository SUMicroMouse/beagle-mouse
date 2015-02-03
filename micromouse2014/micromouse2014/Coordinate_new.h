#ifndef __COORDINATE_NEW_H__
#define __COORDINATE_NEW_H__

/**
* Simple class to hold the x and y grid coordinates of a cell
*/
class Coordinate_new
{
public:
	Coordinate_new()
	{
		this->x = -1;
		this->y = -1;
	}
	Coordinate_new(int X, int Y)
	{
		this->x = X;
		this->y = Y;
	}

private:
	int x;
	int y;
};

#endif