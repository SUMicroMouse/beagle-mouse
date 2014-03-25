//
//  tty.cpp
//  micromouse2014
//
//  Created by Lyle Moffitt on 3/23/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

//#include "tty.h"

using namespace std;

/********************    Teletype Style Device        **********************/

uint8_t * 
device_tty::sleepy_read(size_t num)
{
    uint8_t * tmp = new uint8_t[num+1];
    size_t n_rcvd,ptr=0;
    while(num > 0)
    {
        n_rcvd = read( fd, &(tmp[ptr]), num);
        if (n_rcvd == 0){   
            usleep(tty_config::pkt_lag * num); 
        }else{  
            num -= n_rcvd;
            ptr += n_rcvd;
        }
    }
    return tmp;
}

device_tty::device_tty(string _path):
tty_path (_path)
{
    fd = open( tty_path.c_str(), O_RDWR | O_NOCTTY | O_SYNC | O_NONBLOCK );
    if(fd == -1)
    {   cerr << "Invalid: "<<_path<<"\n";  
        fd=2; 
        tty_path = "/dev/fd/2";
    }
}

template <typename  _type>
inline
_type*
device_tty::rd_(size_t num)
{
    _type* _ret = new _type[num+1];
    size_t _len = read( fd, _ret, num );

    //cout<<"[DBG] "<< tty_path<<" == >> "<< _ret <<endl;
    return _ret;
}

template <typename  _type>
inline
_type
device_tty::rd_()
{
    _type* buf = new _type[1];
    read( fd, buf, 1 );
    
    //printf("[DBG] %s == >> %02X\n",tty_path.c_str(),buf[0]);
    return buf[0]; 
}

template <typename _type>
void
device_tty::wr_(_type val)
{
    strstream _ss;
    _ss << val;
    string _s (_ss.str());
    write(fd,_s.c_str(),_s.length());
    //cout<<"[DBG] "<< tty_path<<" << == "<<_s <<endl;
}