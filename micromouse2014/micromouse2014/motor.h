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

class motor;
class encoder;

namespace motor_config
{
    const std::string mtr[] =
    {
        "/sys/devices/ocp.3/pwm_test_45.11/",//The base bath for motor1
        "/sys/devices/ocp.3/pwm_test_46.13/" //The base bath for motor2
    };
    const std::string enc[] =
    {
        "/dev/path/of/the/encoder/for/mtr[0]",
        "/dev/path/of/the/encoder/for/mtr[1]"
    };
    
    const std::set<const std::string> motor_attr     =
    {
        "period",
        "duty",
        "polarity",
        "run"
    };
    const std::set<const std::string> encoder_attr   =
    {
        "speed",
        "radian"
    };
    
    constexpr    uint max_period    = 10000000;
    constexpr    uint max_speed     = max_period;
    
    enum m_select{
        LEFT    = 0,
        RIGHT   = 1
    };
    
};

class motor
{
    device_dir mtr_dev, enc_dev;
protected:
    
#define make_getter_setter(name)                                    \
    uint name(){      return mtr_dev[#name].gt(uint());    }        \
    void name(uint value){   mtr_dev[#name].st(value);     }        \

    make_getter_setter(period   )
    make_getter_setter(duty     )
    make_getter_setter(polarity )
    make_getter_setter(run      )
    
#undef  make_getter_setter
    
    motor(motor_config::m_select i = motor_config::LEFT);
    
public:
    motor(device_dir _motor, device_dir _encoder);
    
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
