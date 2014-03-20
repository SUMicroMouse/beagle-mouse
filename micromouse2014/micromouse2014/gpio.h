//
//  gpio.h
//  Test
//
//  Created by Lyle Moffitt on 3/19/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#ifndef __Test__gpio__
#define __Test__gpio__

#include <iostream>

#include "device.cpp"

class gpio;
class encoder;

namespace gpio_config
{
    const std::string base_path[] =
    {
        "/sys/class/gpio/gpio69/"
    };

    const std::set<const std::string> attr =
    {
        "direction",    // 'in' -or- 'out'
        "value"         // '1'  -or- '0'
    };
};

class gpio
{
    device_dir pin;
protected:
    
#define make_getter_setter(name)                            \
bool name(){      return pin[#name].gt(uint());    }        \
void name(bool value){   pin[#name].st(value);     }        \

    make_getter_setter(direction  )
    make_getter_setter(value      )
    
#undef  make_getter_setter
    
public:
    gpio(): pin( gpio_config::base_path[0],gpio_config::attr ){}
    
};




#endif /* defined(__Test__gpio__) */
