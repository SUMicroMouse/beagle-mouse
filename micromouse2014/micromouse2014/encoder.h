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
    // Which motor this is referencing; 'L'eft -or- 'R'ight
    char side;
    
    // Relatave position of wheel in mm; 
    encoder_config::position_t    _pos;
    
    // Average velocity of wheel in mm/sec (based on last two positions)
    encoder_config::velocity_t    _vel;
    
    // Average acceleration in mm/sec^2 (based on last two velocities)
    encoder_config::xlr8tion_t    _accl;
};

struct encoder : device_tty
{
    std::deque<encoder_config::snapshot> hist;
    
    encoder(): device_tty( encoder_config::path_ls.begin()[0])
    {}
    
    encoder( const char* _path ):device_tty(_path)
    {}
    
    // Add provided measurements to the histore deque
    inline 
    encoder_config::snapshot 
        add_hist(measure* _L, measure* _R);
    
    // Updates the history with most recent measurements from wheels
    encoder_config::snapshot 
        update();
    
    // Infinite loop of updates
    void loop_update();
};


#endif
