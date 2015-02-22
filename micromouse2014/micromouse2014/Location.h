#pragma once
#ifndef __LOCATION_H__
#define __LOCATION_H__

class Location
{
public:
	Location(){ ; }
	Location(int xx, int yy){ x = xx; y = yy; }
	int x, y;
};

#endif