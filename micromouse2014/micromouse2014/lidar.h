//
//  lidar.h
//  micromouse2014
//
// Created by Carlton Semple, Lyle Moffitt, & Nick Beni
// Created at Syracuse University for the 2014 micromouse competition
//

#ifndef __micromouse2014__lidar__
#define __micromouse2014__lidar__

#include <iostream>
#include <bitset>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <deque>

#include <map>

#include "packet.h"
#include "device.h"

//#include "config.h"
//#include "grid.h"

struct _360_scan;
struct lidar;

typedef std::chrono::time_point<std::chrono::system_clock> timestamp;

namespace lidar_config
{
    constexpr size_t pkt_index_max  = 22;
    
    constexpr char tty_path[]       = "/dev/tty01";
}


// A list of data points organized radially (by degree index)
// Data points are all distances
struct _360_scan
{
    //Stores reference to each data sub-packet composing the 360 degrees
    std::map<uint,const data*> deg_index;
    //The times degree 0 and 359 were entered (respectively)
    timestamp   begin,end;
    
    //deconstructs the packet and stores it in the map.
    size_t add_pkt(packet& _p);
    
    //Print out the list. Template defines whether packet or data(default)
    template <class _t>
    void print();
};






class lidar//: protected device_tty
{
	/** Hold the most recent scan.
	 *	A scan is a radial grid.
	 *	Sample coordinates for cells.
     
     +       + ----- + ----- +
     |               |
     |               |
     |               |
     + ----- +       + ----- +
     |       |
     |       |
     |       |
     +       +       + ----- +
     |               |       |
     |           @   |       |
     |               |       |
     +               +       +
     
     
	 */
    
    // The underlying hardware connection
    device_tty  _dev;

protected:
    std::map<timestamp, _360_scan*> scan_hist;
    
    lidar() : _dev( lidar_config::tty_path ){}
    
public:
	lidar(std::string _path) : _dev( _path ){}
    
    // Scans incoming data up to the first marker ( "0xFA" ) and returns it
	uint8_t seek();
    // Scans a NEW packet of data starting from the marker
    packet& scan(){return scan(seek());}
    packet& scan(uint8_t _seek);
    
    //Addes a new layer to the scan returns the most recent scan
    std::map<timestamp, _360_scan*>::iterator
    build_scan();
    
    //Erase all but the most recent N scans
    void clr_all_but_last(size_t N);
    
	// Returns the scan that is N from most recent
	std::map<timestamp, _360_scan*>::iterator
	fetch_last(size_t N);
	
	
};


#endif /* defined(__micromouse2014__lidar__) */
