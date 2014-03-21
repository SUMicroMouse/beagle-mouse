//
//  gpio.cpp
//  micromouse2014
//
//  Created by Lyle Moffitt on 3/20/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#include "gpio.h"

using namespace std;

gpio(const string _path,
     const set<const string> _attr): 
device_dir( _path , _attr )
{
    
}
