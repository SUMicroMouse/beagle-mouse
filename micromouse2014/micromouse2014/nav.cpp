//
//  nav.cpp
//
// Created by Carlton Semple, Lyle Moffitt, & Nick Beni
// Created at Syracuse University for the 2014 micromouse competition
//


#include "nav.h"


using namespace std;

// Defualt constructor
nav::nav(lidar & lidar_ptr):
view(lidar_ptr),
left(motor_config::m_select::LEFT),
right(motor_config::m_select::RIGHT)
{
    
}


void 
nav::movedistancevariable(size_t mm)
{
    
}


bool 
nav::synchronize(double speed) // should go in go forward
{
    using namespace encoder_config;
    using namespace nav_config;
    
    snapshot * state = enc.hist.front();
    
    auto v_L = abs(get<1>(*state)->_vel);
    auto v_R = abs(get<2>(*state)->_vel);
    auto v_avg = AVG(v_L , v_R);
    
    auto s_L = left.get_speed();
    auto s_R = right.get_speed();
    auto s_avg = AVG(s_L , s_R);
    
    if(eqish(v_avg,speed)    ? 0x01:0){
        left.set_speed (s_avg);
        right.set_speed(s_avg);
        return true;  
    }
    
    if(v_R < v_avg  ? 0x10:0){ right.set_speed(s_R+max_invariance);    }
    if(v_R > v_avg  ? 0x20:0){ right.set_speed(s_R-max_invariance);    }
    if(v_L < v_avg  ? 0x40:0){ left.set_speed (s_L+max_invariance);    }
    if(v_L > v_avg  ? 0x80:0){ left.set_speed (s_L-max_invariance);    }
    return false;
}


// turn by so many degrees, determined by time
// assuming the front middle is 0 degrees
void 
nav::turn(double angle)
{
    using namespace encoder_config;
    using namespace nav_config;
    
    snapshot * state = enc.hist.front();
    
    auto p_L = abs(get<1>(*state)->_pos);
    auto p_R = abs(get<2>(*state)->_pos);
    auto p_avg = AVG(p_L , p_R);
    
    
    
}

void
nav::move(double speed)
{
    
}

void 
nav::veerleft(double ratio)
{
    moveforward();
	left.disable();
	XLR8();
	left.enable();
}


void 
nav::veerright(double ratio)
{
    moveforward();
	right.disable();
	XLR8();
	right.enable();
}


// go forward in a small enough chunk (one cell approx.) so that the distance to the opposing wall can be scanned/updated
void 
nav::goForwardOne()
{
    double val; // give a specifc amount 
                //left motor 
                // right motor both need to advance so both are going in the same direction
	left.enable();
	right.enable();
    
    left.set_speed();
    right.set_speed();
    
    
    left.get_speed();
    right.get_speed();
    
    synchronize();
}


void 
nav::stop()
{
	while(left.chk_en() || right.chk_en() == 1)
	{
	left.disable();
	right.disable();
	}
}


/* loop the three functions that keep the robot going straight */
void nav::stayStraightLoop()
{
	int leftORright;	// -1 means left is closer... 1 means right is closer
	while (true)
	{
		if ((oneSidedApproach() || twoSidedApproach()) && straightAhead(leftORright))
		{	
			// fix it
			switch (leftORright)
			{
			case -1:	// nudge left
				break;
			case 1:		// nudge right
				break;
			default:
				break;
			}
		}
	}
}

/* Return true if the function determines that the left center is closer than the right center,
or if the right center is closer than the left center */
bool nav::straightAhead(int &leftORright)
{
	using namespace lidar_config;

	// the lidar is called view
	auto scanPoint = view.scan_hist.front();

	int left = (degree_north - 1 + degree_max) % degree_max;
	int right = (degree_north + 1 + degree_max) % degree_max;

	auto p_OfInterest = scanPoint->deg_index.at(degree_north);	// straight ahead
	
	p_OfInterest->

	return false;
}
