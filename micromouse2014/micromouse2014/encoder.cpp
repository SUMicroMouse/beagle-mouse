//
//  encoder.cpp
//  micromouse2014
//
//  Created by Lyle Moffitt on 3/20/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#include "encoder.h"

using namespace std;
using namespace encoder_config;

encoder_config::snapshot 
encoder::add_hist(measure *_L, measure *_R)
{
    auto _t = std::chrono::system_clock::now().time_since_epoch().count();
    hist.push_front(tuple<timestamp,measure*, measure*>(_t, _L, _R));

    if (hist.size() > hist_max){   hist.pop_back();    }
    return hist.front();
}

snapshot
encoder::update()
{
#define FAIL_CHK                                    \
do{                                                 \
    if(m_ptr == nullptr)                            \
    {                                               \
        cerr << "\nTranslation failure!"<<endl;     \
        throw exception();                          \
    }                                               \
                                                    \
}while(0)

    uint8_t *msg = new uint8_t[msg_size];
    msg = (*this).sleepy_read(msg_size);
    measure *left, *right, *m_ptr=nullptr;
    for (size_t i=0; i<msg_size; ++i) 
    {
        switch (msg[i]) 
        {
            case 'L': m_ptr = left  = new measure(); break;
            case 'R': m_ptr = right = new measure(); break;
            case 'P': 
                ++i; FAIL_CHK;  m_ptr->_pos = * (position_t*)msg; 
                i += sizeof(position_t); break;
            case 'V': 
                ++i; FAIL_CHK;  m_ptr->_vel = * (velocity_t*)msg; 
                i += sizeof(velocity_t); break;
            case 'A': 
                ++i; FAIL_CHK;  m_ptr->_accl = * (xlr8tion_t*)msg; 
                i += sizeof(xlr8tion_t); break;
                
            case EOE:  return add_hist(left, right);
        }
    }
    delete msg;
#undef FAIL_CHK
    return snapshot(NULL,nullptr,nullptr);
}

void 
encoder::loop_update()
{
    while (true) 
    {
        update();
        usleep(msg_freq_microsec);
    }
}





