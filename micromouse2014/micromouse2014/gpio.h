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
        "/sys/class/gpio/gpio74/"
    };

    const std::set<const std::string> attr =
    {
        "direction",    // 'in' -or- 'out'
        "value"         // '1'  -or- '0'
    };
};

struct gpio : device_dir
{
    
#define make_getter_setter(name)                            \
bool name(){      return (*this)[#name].gt<uint>();    }    \
void name(std::string val){   (*this)[#name].st(val);  }    \

    make_getter_setter(direction  )
    make_getter_setter(value      )
    
#undef  make_getter_setter
    
    gpio(const std::string _path,
         const std::set<const std::string> _attr);    
};




#endif /* defined(__Test__gpio__) */
