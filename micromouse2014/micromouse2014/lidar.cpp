//
//  lidar.cpp
//  micromouse2014
//
//  Created by Lyle Moffitt on 1/21/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#include "lidar.h"

using namespace std;

/***********************      SCAN_MAP METHODS    ***********************/

size_t
_360_scan::add_pkt(packet& _p)
{
    using namespace packet_config;
    
    if (_p.index == index_min) { begin = _p.p_time; }
    if (_p.index == index_max) { end = _p.p_time; }

    uint degree = (_p.index - index_min) * data_num;
    deg_index[degree + 0]= &_p.d0 ;
    deg_index[degree + 1]= &_p.d1 ;
    deg_index[degree + 2]= &_p.d2 ;
    deg_index[degree + 3]= &_p.d3 ;
//    deg_index.emplace( pair<uint,data>(degree + 0, _p.d0) ) ;
//    deg_index.emplace( pair<uint,data>(degree + 1, _p.d1) ) ;
//    deg_index.emplace( pair<uint,data>(degree + 2, _p.d2) ) ;
//    deg_index.emplace( pair<uint,data>(degree + 3, _p.d3) ) ;
    
    return deg_index.size();
}

template <class _t>
void
_360_scan::print()
{
    for(auto it = deg_index.begin(); it != deg_index.end();it++)
    {
        if( (it->first%4 ==0) && (typeid(_t)==typeid(packet)) )
            it->second->source->print();
        it->second->print(it->first%4);
    }
}


/***********************      LIDAR METHODS    **************************/
uint8_t
lidar::seek()
{
    uint8_t _c=0;
    //while( 0xFA != _c ){ _c = _dev.rd_<uint8_t>(1); }
    return _c;
}

packet&
lidar::scan(uint8_t _seek)
{
    using namespace lidar_config;
    vector<uint8_t> _v;// (pkt_index_max,0);
    _v.push_back(_seek);
    //vector<uint8_t> _c ( _dev.rd_<vector<uint8_t>>(pkt_index_max-1) );
    //_v.insert(_v.end(), _c.begin(), _c.end());
    //for( int i =0; i<(pkt_index_max-1) ; ++i ){_v[1+i] = _u[i];}
    return *new packet (_v);
}

std::map<timestamp, _360_scan*>::iterator
lidar::build_scan()
{
    auto _l = new _360_scan;
    while ( _l->add_pkt(scan()) < 360);
    return scan_hist.insert(scan_hist.end(),
                            pair<timestamp, _360_scan*>
                            (_l->end, _l) );
}

void
lidar::clr_all_but_last(size_t N)
{
    scan_hist.erase(scan_hist.begin(), fetch_last(N));
}

std::map<timestamp, _360_scan*>::iterator
lidar::fetch_last(size_t N)
{
    N = (N>(scan_hist.size()-1)?(scan_hist.size()-1):N);
    auto it = scan_hist.end();
    while (N>0) {it--;}
    return it;
}









