#include <iostream>
#include "nav.h"
#include "motor.h"

using namespace std;

void nav::curveleft(float VR, float speed)
{
	VL = 5; 



};

void nav::turnleft(float VR, float VL)
{

	VR = 5;
	VL = -5;

};

void nav::turnright(float VR, float VL)
{

	VR = -5;
	VL =  5;

};


void nav::moveforward(float VR, float VL)
{
	
	VR = 5;
	VL = 5;


};

void nav::movebackward(float VR, float VL)
{

	VR = -5;
	VL = -5;



};

void nav::coast(float VR, float VL)
{


	VR = VR/2;
	VL = VL/2;




};

void nav::XLR8(float VR, float VL)
{

	VR = VR*2;
	VL = VL*2;


};


void nav::veerleft(float VR, float VL)
{





};


void nav::veerright(float VR, float VL)
{





};

void nav::stop(float VR, float VL)
{

	VR = 0;
	VL = 0;


};