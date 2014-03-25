//
//  lidar.cpp
//  micromouse2014
//
//  Created by Lyle Moffitt on 1/21/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#include "lidar.h"

using namespace std;




/***********************      LIDAR METHODS    **************************/





uint8_t
lidar::seek()
{
	//cout << "\nSeeking...\n";
    uint8_t _c=0;
	while( 0xFA != _c ){ 
		_c = _dev.rd_<uint8_t>();
		//usleep(10); 
		}
	//cout << "Found!\n";
    return _c;
}

packet*
lidar::scan(uint8_t _seek)
{
    using namespace lidar_config;
    vector<uint8_t> _v;// (pkt_index_max,0);
    _v.push_back(_seek);
	uint8_t * _n =  _dev.sleepy_read(pkt_index_max-1);
    //_v.insert(_v.end(), _n, pkt_index_max-1);
    for( int i =0; i<(pkt_index_max-1) ; ++i ){ _v.push_back(_n[i]); }
    return new packet (_v);
}

_360_scan*
lidar::build_scan()
{
    _360_scan * _l = new _360_scan;
    while ( _l->add_pkt(scan()) < 360);
    scan_hist.push_front(_l);
    if(scan_hist.size() > lidar_config::hist_max){ scan_hist.pop_back(); }
    return  _l;

}
