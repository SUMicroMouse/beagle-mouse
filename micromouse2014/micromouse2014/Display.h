#pragma once
#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "Maze.h"

class Display
{
public:
	void print(); // print info
	void collect(); // unsure of functionality atm..
private:
	Maze* maze;
};

#endif