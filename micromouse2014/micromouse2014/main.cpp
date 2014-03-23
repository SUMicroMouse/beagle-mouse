//
//  main.cpp
//  micromouse2014
//
// Created by Lyle Moffitt, Carlton Semple, & Nick Beni
// Created at Syracuse University for the 2014 micromouse competition
//

//#include "robot.h"
//#include "device.h"
//#include "gpio.h"

//#include "../test.h"
#include "motor.h"

#include "lidar.h"

#include <iostream>
#include <string>

namespace test
{
    static auto path_ls = 
    {
        "/Users/Lyle/Desktop/test1/",
        "/Users/Lyle/Desktop/test2/"
    };
    
    static auto attr_ls =
    {
        "t1",
        "t2"
    };
}


int main(int argc, const char * argv[])
{    
    gpio dd ( test::path_ls.begin()[0] , test::attr_ls);
	
    dd["t1"].st("hello world");
    
    std::string ret = dd["t1"].gt<std::string>();
    
    std::cout<<ret<<std::endl;
    
    
    return 0;
}

