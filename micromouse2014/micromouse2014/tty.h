//
//  tty.h
//  micromouse2014
//
//  Created by Lyle Moffitt on 3/23/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#ifndef __micromouse2014__tty__
#define __micromouse2014__tty__

#include <iostream>
#include <strstream>
#include <string>


#ifndef _WIN32

#include <termios.h>
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h> // File control definitions

#endif //_WIN32

namespace tty_config
{
	constexpr size_t BAUD =115200;
	constexpr size_t pkt_lag = std::micro::den * float(1.0/BAUD);
};


/********************    Teletype Style Device        **********************/
class device_tty
{
    // The full path to the device, including its name
    std::string tty_path;// == "/dev/ttyO1"
    
    // File handle for the specific tty
	int     fd;
protected:
    device_tty():tty_path(""){ fd=-1; }
    
public:
    
	device_tty(std::string _path);
    
    device_tty(const char* _path):device_tty(std::string(_path))
    {}
    ~device_tty(){ close(fd); }
    
    uint8_t * sleepy_read(size_t num);
    
	template <typename _type>
    _type   rd_();
    
    template <typename _type>
    _type*  rd_(size_t num);
    
    template <typename _type>
	void	wr_(_type val);
    
};

#include "tty.cpp"



#endif /* defined(__micromouse2014__tty__) */