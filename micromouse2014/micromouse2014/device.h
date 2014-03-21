//
//  device.h
//  Test
//
//  Created by Lyle Moffitt on 3/6/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#ifndef Test_device_h
#define Test_device_h

#include <iostream>
#include <fstream>
#include <string>
#include <strstream>
#include <vector>
#include <set>

//#include <typeinfo>
//#include <typeindex>

#ifndef _WIN32

#include <termios.h>
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h> // File control definitions

#endif //_WIN32


struct dev
{
    dev(std::string _n):dev_path(_n){}
    
    const std::string dev_path;
    
    template<typename _type>
    void st(_type value);
    
    template<typename _type>
    _type gt();
    
};

/********************     Directory Style Device       *********************/
class device_dir
{
    // The path to the root directory of the device file-tree
	const
    std::string dir_path;
    // The device attribute(s) to be controlled.
    const
    std::set<std::string> dev_attr;
    
protected:
    friend dev;
    std::string chk_name(std::string  _name);
    
public:
	device_dir(std::string _base, 
               std::set<std::string>  _attr);
    
    dev operator[] (std::string _attr);

};


/********************    Teletype Style Device        **********************/
class device_tty
{
    // The full path to the device, including its name
    const
    std::string tty_path ;// == "/dev/ttyO1"
    
    // File handle for the specific tty
	int     fd;
protected:
    device_tty():tty_path(""){ fd=-1; }
    
public:
    
	device_tty(std::string _path);
    
	template <typename _type>
    _type   rd_(size_t num);
    
    template <typename _type>
	void	wr_(_type val);
    
};





#endif
