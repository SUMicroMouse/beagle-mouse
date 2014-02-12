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


//#include "config.h"
//#include "grid.h"

struct grid{};


#define timestamp long unsigned

#define coeff_bit_len 6
#define expnt_bit_len 16 - coeff_bit_len

#define LIDAR_DEPTH 5

template<size_t _num, size_t _wid >
struct bitstr
{
//    typedef bitstr<_num, 1> binstr;
//    typedef bitstr<_num, 3> octalstr;
//    typedef bitstr<_num, 4> hexstr;
    //Data
    std::vector<std::bitset<_wid>> v_0b;
    //Constructors
    bitstr();
    bitstr(std::string str, int base);
    //Methods
    template<size_t _bits>
    std::bitset<_bits> get_segment(size_t bit_offset);
};


class data
{
    //4 bytes, 32 downto 0
    std::bitset<32> _0b;
public:
    
    data(int _d);
    data(uint _d);

    uint16_t    distance();//13 : 0 (LSB first)
    bool        strength_warn();//b14
    bool        invalid();//b15
    uint16_t    strength();//31 : 16 (LSB first)

};

class packet
{
    std::vector<uint8_t> _raw;
public:
    //Order of receipt
    uint8_t     start();//0xFA
    uint16_t    index();//Packet #0 - #89, 4 readings each
    float       speed();//16b total, 10.6b fixed, 64th of RPM
    data        data0();
    data        data1();
    data        data2();
    data        data3();
    uint16_t    chksum();
    
};




float fix_to_float(std::bitset<coeff_bit_len> coeff,
                   std::bitset<expnt_bit_len> expnt);

class lidar
{
	static const
	unsigned depth = LIDAR_DEPTH;
	
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
    
	// Holds the most recent scan
	grid & scan;
	// holds a temporary grid while the scan is being updated
	grid & temp_scan;
    // The file stream device that will be read from to generate the grid.
    std::ifstream f_data;
    
    // The name of the file. Probably "/dev/tty01"
    std::string f_name;
	
    //Returns a 44 character hex string, starting with (0x) "FA"
    std::string fetch_datagram();
    
	void seek_to_start();
	 
    int open_file();
    int close_file();
    
public:
	lidar();
    

    
    
	
	/** Scans the field in front are returns a *radial* grid of cells.
	 *	1)	check that polling() == false
	 *	2)	check that current() == false
	 */
	void update_scan();
	
	/// Returns the most recent scan
	grid &	fetch_scan();
	
	/// Returns true while there is no new scan to get.
	bool	current();
	
	/// Returns true if the grid is unfinished
	bool	polling();
	
};


#endif /* defined(__micromouse2014__lidar__) */
