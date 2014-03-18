
#include <iostream>
#include "config.h"

class packet{
	double m1;
	double m2;
	double m3;
	double m4;
public:
	double aveRadius;
	int angle;

	packet();
	packet(double one, double two, double three, double four, int i);
	
	void fill(double a, double b, double c, double d);
};



