
#include <iostream>
#include "config.h"

class wall{
public:
double length;
double viewAngle; // angle between the two vectors
double leftAngle;
double rightAngle;

// the measurements that helped create the wall
double radiusLeft;
double radiusRight;
double a1;
double a2;

double xBegin, xEnd; // when the wall is computed, this is the final step in capturing where it is in relation to cells
double yBegin, yEnd;

double topTriangleAngle; // used when computing the parallel/perpendicular distances. it's the viewAngle 
double perpendicularDistance;
double parallelDistance;

	wall() { length = 0; }
	wall(double difference) { length = difference; }
	wall(double radius1, double radius2, double angle1, double angle2);
	
};



