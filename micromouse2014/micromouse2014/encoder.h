//
//  encoder.h
//  Test
//
//  Created by Lyle Moffitt on 3/19/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#ifndef Test_encoder_h
#define Test_encoder_h

#include "tty.h"

#include <deque>
#include <utility>

class encoder;
struct measure;


namespace encoder_config
{    
    static auto path_ls   =
    {
        "/dev/ttyO2"
    };

    typedef std::chrono::time_point<std::chrono::system_clock>::rep timestamp;

    typedef int16_t position_t;
    typedef int16_t velocity_t;
    typedef int16_t xlr8tion_t;
    
    constexpr size_t hist_max = 100;
    
    constexpr size_t msg_size = ( (sizeof(position_t)+sizeof(uint8_t))
                                 +(sizeof(velocity_t)+sizeof(uint8_t))
                                 +(sizeof(xlr8tion_t)+sizeof(uint8_t))
                                 + sizeof(uint8_t)*2
                                 );
    
    constexpr size_t msg_freq_microsec =  250;

    constexpr uint8_t EOE/*End-Of-Encoding*/ = '\0';
    
    typedef std::tuple< timestamp, measure*, measure* > snapshot ;
};

struct measure
{
    char side;
    encoder_config::position_t    _pos;
    encoder_config::velocity_t    _vel;
    encoder_config::xlr8tion_t    _accl;
};

struct encoder : device_tty
{
    std::deque<encoder_config::snapshot> hist;
    
    encoder(): device_tty( encoder_config::path_ls.begin()[0])
    {}
    
    encoder(const char* _path):device_tty(_path)
    {}
    
    inline 
    encoder_config::snapshot 
        add_hist(measure* _L, measure* _R);
    
    encoder_config::snapshot 
        update();
    
    void loop_update();
};


#endif
