//
//  encoder.h
//  Test
//
//  Created by Lyle Moffitt on 3/19/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#ifndef Test_encoder_h
#define Test_encoder_h

#include "device.cpp"

class encoder;

namespace encoder_config
{
    const std::string base_path[] =
    {
        "/sys/devices/ocp.*/48302000.epwmss/48302180.eqep/"
        "/sys/devices/ocp.*/48304000.epwmss/48304180.eqep/"
    };
    static auto path_ls   =
    {
        "/dev/path/of/the/encoder/for/mtr[0]",
        "/dev/path/of/the/encoder/for/mtr[1]"
    };
    
    static auto attr_ls =
    {
        "enabled",  // Enable hardware (0/1) = off/on
        "mode"      // Capture mode (0/1) = Absolute/Relative
                    //      Absolute - The position is continually incr/decr (max 2^32)
                    //      Relative - The position is reset after each period
        "period",   // How often an 'overflow' occurs (nanoseconds) (max 40s)
                    // How often the process thread wakes (if position is polled)
        "position"  // 
    };
};

class encoder
{
    device_dir pin;
protected:
    
#define make_getter_setter(name)                            \
bool name(){      return pin[#name].gt(uint());    }        \
void name(bool value){   pin[#name].st(value);     }        \

    make_getter_setter(enabled      )
    make_getter_setter(mode         )
    make_getter_setter(period       )
    make_getter_setter(position     )
    
#undef  make_getter_setter
    
public:
    encoder(): pin( encoder_config::base_path[0],encoder_config::attr ){}
    
};


#endif
