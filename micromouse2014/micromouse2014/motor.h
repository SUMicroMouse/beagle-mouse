//
//  motor.h
//  Test
//
//  Created by Lyle Moffitt on 3/9/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#ifndef __Test__motor__
#define __Test__motor__

#include <iostream>
//#include "device.h"
#include "device.cpp"
#include "gpio.h"

class motor;
class encoder;

namespace motor_config
{
    static auto mtr_path =
    {
        "/sys/devices/ocp.3/pwm_test_45.11/",//The base bath for motor1
        "/sys/devices/ocp.3/pwm_test_46.13/" //The base bath for motor2
    };
    static auto enc_path =
    {
        "/dev/path/of/the/encoder/for/mtr[0]",
        "/dev/path/of/the/encoder/for/mtr[1]"
    };
    
    static auto mtr_attr   =
    {
        "period",
        "duty",
        "polarity",
        "run"
    };
    static auto enc_attr   =
    {
        "speed",
        "radian"
    };
    
    constexpr    uint max_period    = 10000000;
    constexpr    uint max_speed     = max_period;
    
    enum m_select
    {
        LEFT    = 0,
        RIGHT   = 1
    };
    
};

class motor
{
    device_dir mtr_dev;//, enc_dev;
    gpio enabler;
    
protected:

#define make_getter_setter(name, type, i)           \
type name(){    return mtr_dev[#name].gt<i>();  }   \
void name(type val){   mtr_dev[#name].st(val);  }   \

    make_getter_setter(period    ,int ,int )
    make_getter_setter(duty      ,int ,int )
    make_getter_setter(polarity  ,int ,int )
    make_getter_setter(run       ,int ,int )
    
#undef  make_getter_setter

    
    
public:

    
//template<typename... Args> motor(Args&&... args):mtr_dev( args... ){}
    
    motor(motor_config::m_select side):
        mtr_dev(motor_config::mtr_path.begin()[side], 
                motor_config::attr_ls),
        enabler(side)
    {}
    
    //motor(device_dir _motor, device_dir _encoder, gpio _enabler);
    
    // Set speed as ratio of maximum
    void set_speed(double _val);
    
    // Get speed of motor as ratio of maximum
    double get_speed();
    
    /** Returns the angular position of the attached wheel.
     *  The origin this arc is measured in reference to is unknown.
     *  The distance units for this measurement are unknown.
     *  With consistent movement CW or CCW, the position will increment
        or decrement consistently upto an unknown (but very large) value,
        at which point it will reset.
     *  Clockwise (CW) may be associated with forward movement and an
        increase in this value.
     *  Do not rely heavily on this function.
     */
    size_t get_angular_position();
    
    // Turn the motor on ; apply voltage 
    void enable();
    
    // Turn the motor off; cost
    void disable();
    
    // Check to see if the motor is enabled (true) or disabled
    bool chk_en();
    
    
    
};


#endif /* defined(__Test__motor__) */
