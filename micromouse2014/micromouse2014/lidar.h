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
#include "tty.cpp"

//#include "config.h"
//#include "grid.h"

struct _360_scan;
struct lidar;

typedef std::chrono::time_point<std::chrono::system_clock> timestamp;

/// Device constants specific to the XV-11 LIDAR
namespace lidar_config
{
    /// The number of bytes in the packet
    constexpr size_t pkt_index_max  = 22;
    
    /// The full path to the UART terminal the LIDAR is connected to.
    static auto tty_path       = "/dev/tty01";
    
    /// The reference degrees for the distance measurements
    constexpr uint degree_north     = 90;
    constexpr uint degree_south     = 270;
    constexpr uint degree_east      = 0;
    constexpr uint degree_west      = 180;
}


/** A list of DATA points organized radially by degree {0-359}.
 */
struct _360_scan
{
    ///Stores pointer to each data sub-packet composing the 360 degrees
    std::map<uint,const data*> deg_index;
    
    ///The times that degree 0 and 359 were entered (respectively)
    timestamp   begin,end;
    
    ///deconstructs the packet and stores it in the map.
    size_t add_pkt(packet& _p);
    
    ///Print out the list. Template defines whether packet or data(default)
    template <class _t>
    void print();
};





/** The top-level interface for using the LIDAR
 */
class lidar//: protected device_tty
{
    /// The underlying hardware device connection
    device_tty  _dev;

protected:
    /// A list of previous scans, organized by time taken.
    std::map<timestamp, _360_scan*> scan_hist;
    
    /// Defualt constructor relys on namespace constants.
    lidar() : _dev( lidar_config::tty_path ){}
    
public:
    /// Explicit constructor requires that UART terminal path be provided. 
	lidar(std::string _path) : _dev( _path ){}
    
    /// Scans incoming data up to the start ( "0xFA" ) and returns that.
	uint8_t seek();
    
    /// Scans in a NEW packet starting from 0xFA
    packet& scan(){return scan(seek());}
    packet& scan(uint8_t _seek);
    
    /// Addes a new entry to the scan history; returns the most recent
    std::map<timestamp, _360_scan*>::iterator
    build_scan();
    
    /// Erase all but the most recent N scans
    void clr_all_but_last(size_t N);
    
	/// Returns the scan that is N from most recent
	std::map<timestamp, _360_scan*>::iterator
	fetch_last(size_t N);
	
    _360_scan*
    last_scan();
    
	/** TODO: Scan history interface.
     There will probably need to be more methods for using the scan history.
     It will build rather fast (about 4-5 scans/second), so we need to 
     figure out how to deal will all that information backlog.
     */
};


#endif /* defined(__micromouse2014__lidar__) */
