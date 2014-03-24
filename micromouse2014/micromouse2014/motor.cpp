//
//  motor.cpp
//  Test
//
//  Created by Lyle Moffitt on 3/9/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#include "motor.h"

using namespace motor_config;



void 
motor::set_speed(double _val)
{
    _val = (_val > 1? 1:
            _val < 0? 0:
            _val);
    duty( _val * motor_config::max_speed );
}


double 
motor::get_speed()
{
    //place holder; do assume this is the actual body!
    return  double(duty()) / double(motor_config::max_speed) ;
}


size_t 
motor::get_angular_position()
{
    return 1;
}

void 
motor::enable()
{
    enabler.direction("out");
    enabler.value("1");
}

void 
motor::disable()
{
    enabler.direction("out");
    enabler.value("0");
}

bool 
motor::chk_en()
{
    enabler.direction("in");
    return enabler.value();
}



