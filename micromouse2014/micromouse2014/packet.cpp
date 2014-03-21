#include "packet.h"

/* ********************             DATA            ******************** */
const
data::data(const packet * _p, std::vector<uint8_t> _d):
distance        (_d[0] | (_d[1]&0x3F)<<8 ),
strength_warn   (_d[1] & 0x40),
invalid_data    (_d[1] & 0x80),
strength        (_d[2] | _d[3]<<8 ),
source          (_p)
{}

const
data::data(data & _d):
distance        (_d.distance),
strength_warn   (_d.strength_warn),
invalid_data    (_d.invalid_data),
strength        (_d.strength),
source          (_d.source)
{}


void 
data::print(const uint index) const
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


/* ******************               PACKET            ****************** */

packet::packet(std::vector<uint8_t> _v):
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
    int16_t b=0,a =( _v[2]<<8 | _v[3] );
    for (uint16_t _u = 0x0001; _u&0xFFFF; _u<<=1){ b= (b<<1) + (_u & a); }
    speed 	= double(b) / 64.0;
    
    // Recalculate the checksum
    std::vector<uint16_t> _c;
    for (int i =0; i< 10 ; ++i){ _c.push_back( _v[2*i] | _v[2*i+1]<<8 ); }
    int32_t chk32=0;
    for (auto it=_c.begin(); it != _c.end(); chk32 = (chk32<<1) + *it++);
    chkcalc = 0x7FFF & ((chk32 & 0x00007FFF) + (chk32 >> 15));
}


void 
packet::print() const
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













