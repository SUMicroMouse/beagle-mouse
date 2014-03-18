#include "wall.h"
#include <math.h>

wall::wall(double radius1, double radius2, double angle1, double angle2)
{
	radiusLeft = radius1;
	radiusRight = radius2;

	viewAngle = angle2 - angle1;
	length = sqrt(pow(radius1, 2.0) + pow(radius2, 2.0) - 2 * radius1 * radius2 * cos(viewAngle)); // length of wall. a^2 = b^2 + c^2 - 2bc * cos(A)
	
	rightAngle = acos((pow(radius1, 2.0) + pow(radius2, 2.0) + pow(length, 2.0)) / (2 * radius2 * length)); // see angle B in diagram. B = arcos((b^2 + c^2 + a^2) / 2ca)
	leftAngle = 180 - viewAngle - rightAngle;

	// calculate perpendicular/parallel distances from the position of the robot to the start of the wall
	topTriangleAngle = 180 - leftAngle;
	perpendicularDistance = sin(topTriangleAngle) * radius1; // radius 1 is the beginner one, which is naturally the closer vector to the robot
	parallelDistance = cos(topTriangleAngle) * radius1;


	// determine if the wall is parallel with the robot or perpendicular, using if farAngle is greater or smaller than midAngle



	xBegin = -1;
	xEnd = -1;
	yBegin = -1;
	yEnd = -1;
}