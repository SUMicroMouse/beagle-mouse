#pragma once
#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "Maze.h"

class Display
{
public:
	Display();
	Display(Maze m) { maze = m; }
	void print(); // print info
	void collect(); // collect will gather all information from Sensor and Motor and update Maze
private:
	Maze maze;
};

#endif