//
//  tty.cpp
//  micromouse2014
//
//  Created by Lyle Moffitt on 3/23/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#include "tty.h"

using namespace std;

/********************    Teletype Style Device        **********************/
device_tty::device_tty(string _path):
tty_path (_path)
{
    fd = open( tty_path.c_str(), O_RDWR);// | O_NOCTTY | O_SYNC | O_NONBLOCK );
    if(fd == -1)
    {   cerr << "Invalid: "<<_path<<"\n";  
        fd=2; 
        tty_path = "/dev/fd/2";
    }
}

template <typename  _type>
_type
device_tty::rd_(size_t num)
{
    char* buf = new char[num+1];
    size_t _len = read( fd, buf, num );
    _type _ret;
    try{
        do{
            for(size_t i=0; i<_len ; _ret.push_back(buf[i++]) ){}
            if((num -= _len) > 0){ _len = read(fd, buf, num); 
            }else{ break; }
        }while( true );
    }catch(...){ cerr << "ERROR" << endl; }
    cout<<"[DBG] "<< tty_path<<" == >> "<< _ret <<endl;
    return _ret;
}

template <typename  _type>
_type
device_tty::rd_()
{
    char* buf = new char[1];
    read( fd, buf, 1 );
    
    cout<<"[DBG] "<< tty_path<<" == >> "<< buf[0] <<endl;
    return _type(buf[0]); 
}

template <typename _type>
void
device_tty::wr_(_type val)
{
    strstream _ss;
    _ss << val;
    string _s (_ss.str());
    write(fd,_s.c_str(),_s.length());
    cout<<"[DBG] "<< tty_path<<" << == "<<_s <<endl;
}
