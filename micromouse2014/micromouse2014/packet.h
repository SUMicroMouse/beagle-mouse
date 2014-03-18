//
//  packet.h
//  Test
//
//  Created by Lyle Moffitt on 3/7/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#ifndef Test_packet_h
#define Test_packet_h

#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>

struct data;
struct packet;

/// Specific hard-coded values for the xv-11 LIDAR packet
namespace packet_config
{
    constexpr uint index_min     = 0xA0;
    constexpr uint index_max     = 0xF9;
    constexpr uint index_range   = 90;
    
    constexpr uint data_num = 4;
}

/** The 4-Byte packet sub-component with primary data.
 
 It is organized Least Significant BYTE (LSB) first, but each byte is 
 organized Most Significant *bit* (MSb) first.
 
 The Byte sequence is:
    B0 :    <distance [7:0]>
    B1 :    <"invalid data" flag [15]> 
            <"strength warning" flag [16]>
            <distance [13:8]>
    B2 :    <signal strength [7:0]>
    B3 :    <signal strength [15:8]>
 */
struct data
{
    /// Preferred constructor
    const
    data(const packet * _p, std::vector<uint8_t> _d):
        distance        (_d[0] | (_d[1]&0x3F)<<8 ),
        strength_warn   (_d[1] & 0x40),
        invalid_data    (_d[1] & 0x80),
        strength        (_d[2] | _d[3]<<8 ),
        source          (_p)
    {}
    
    /// Copy constructor, if necessary
    const
    data(data & _d):
        distance        (_d.distance),
        strength_warn   (_d.strength_warn),
        invalid_data    (_d.invalid_data),
        strength        (_d.strength),
        source          (_d.source)
    {}
    
    /// The distance to the point of reflection measured in milimeters.
    const uint16_t      distance;        
    
    /** Warning when the reported strength is greatly inferior to what
        is expected at this distance.
     
     Possible causes:
        * Material is too dark.
        * Incomplete surface (edge, corner, too bumpy).
        * Material is translucent.
     */
    const bool          strength_warn;    
    
    /** Indicates that the distance could not be calculated.
     
     When triggered, 'distance' will be an error code, for ex:
        0x02, 0x03, 0x21, 0x25, 0x35 or 0x50...
     */
    const bool          invalid_data;     
    
    /// Strength of the signal
    const uint16_t      strength;         //31 : 16
    
    /// Pointer to the containing packet
    const packet *      source;
    
    
    void print(const uint index) const
    {
        using namespace std;
        cout << "\nData "<<index<<":";
        cout << "\n     Dist. : ";
        if (!invalid_data) cout << distance;
        else printf("INVALID : %04X",distance);
        cout << "\n     Strn. : ";
        if (!strength_warn ) cout << strength;
        else cout << "INVALID";
        return ;
    }
    
 };


/** The full 22-Byte packet received from the lidar.
 
 It is organized Least Significant BYTE (LSB) first, but each byte is 
 organized Most Significant *bit* (MSb) first.
 
 Orgnaized as:
    B{0-1}  : <start> <index> 
    B{2-3}  : <speed_L> <speed_H> 
    B{...}  : [Data0 {4-7}] [Data1 {8-11}] [Data2 {12-15}] [Data3 {16-19}]
    B{20-21}: <checksum_L> <checksum_H>
 */
struct packet
{
    /// Packet constructor
    packet(std::vector<uint8_t> _v):
        d0(this, { _v[ 4],_v[ 5],_v[ 6],_v[ 7] }),
        d1(this, { _v[ 8],_v[ 9],_v[10],_v[11] }),
        d2(this, { _v[12],_v[13],_v[14],_v[15] }),
        d3(this, { _v[16],_v[17],_v[18],_v[19] }),
        start(_v[0]),
        index(_v[1]),
        chksum(_v[21]<<8 | _v[20]),
        p_time(std::chrono::system_clock::now())
    {
        // Calculate the speed
        int16_t b,a =( _v[2]<<8 | _v[3] );
        for (uint16_t _u = 0x0001; _u&0xFFFF; _u<<=1){ b= (b<<1) + (_u & a); }
        speed 	= double(b) / 64.0;
        
        // Recalculate the checksum
        std::vector<uint16_t> _c;
        for (int i =0; i< 10 ; ++i){ _c.push_back( _v[2*i] | _v[2*i+1]<<8 ); }
        int32_t chk32=0;
        for (auto it=_c.begin(); it != _c.end(); chk32 = (chk32<<1) + *it++);
        chkcalc = 0x7FFF & ((chk32 & 0x00007FFF) + (chk32 >> 15));
    }
    
    /// The time the packet was constructed, as measured by the OS
    const std::chrono::time_point<std::chrono::system_clock> p_time;
    
    /// The Start-byte (always 0xFA)
    const uint8_t    start;
    
    /** The packet index number out of 90 total.
     
     Literally enumerated {0xA0 - 0xF9}, representing #0 - #89.
     */
    const uint8_t    index;
    
    /// The rotational speed of the lidar measured as 64th of RPM 
    double      speed;
    
    /// The primary data information of the packet.
    const data       d0,d1,d2,d3;
    
    /// The *contained* checksum value
    const uint16_t    chksum;// == chkcalc (?)
    
    /// A checksum calculated from the received information.
    uint16_t    chkcalc;// == chksum (?)
    
    
    void print() const
    {
        using namespace std;
        auto val = p_time.time_since_epoch().count();
        printf("\nConstents of packet #%016llX",val);
        cout << "\nStart byte : " << start;
        cout << "\nIndex byte : " << index;
        cout << "\nSpeed value: " << speed;
        d0.print(0);
        d1.print(1);
        d2.print(2);
        d3.print(3);
        cout << "\nChecksum: ";
        if (chkcalc==chksum) cout<<"MATCH";
        else printf("ERR %04X != %04X",chkcalc,chksum);
    }
    
};




#endif
