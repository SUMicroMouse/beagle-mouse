//
//  motor.cpp
//  Test
//
//  Created by Lyle Moffitt on 3/9/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#include "motor.h"

using namespace motor_config;

motor::motor(m_select i):
mtr_dev(mtr[i],motor_attr),
enc_dev(enc[i],encoder_attr),
enabler(gpio_config::base_path[i],
        gpio_config::attr)
{
    
}

void 
motor::set_speed(double _val)
{
    duty( _val * max_speed );
}


double 
motor::get_speed()
{
    return duty();
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