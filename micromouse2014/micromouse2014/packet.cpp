#include "packet.h"

/* ********************             DATA            ******************** */
data::data(packet * _p, std::vector<uint8_t> _d):
distance        (_d[0] | (_d[1]&0x3F)<<8 ),
strength_warn   (_d[1] & 0x40),
invalid_data    (_d[1] & 0x80),
strength        (_d[2] | _d[3]<<8 ),
source          (_p)
{}

data::data(data & _d):
distance        (_d.distance),
strength_warn   (_d.strength_warn),
invalid_data    (_d.invalid_data),
strength        (_d.strength),
source          (_d.source)
{}


void 
data::print(uint index) const
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
p_time(std::chrono::system_clock::now().time_since_epoch().count())
{
    // Calculate the speed
    int16_t b=0,a =( _v[2]<<8 | _v[3] );
    for (uint16_t _u = 0x0001; _u&0xFFFF; _u<<=1){ b= (b<<1) + (_u & a); }
    speed 	= b; /// 64.0;
    
    // Recalculate the checksum
    std::vector<uint16_t> _c;
    for (int i =0; i< 10 ; ++i){ _c.push_back( _v[2*i] | _v[2*i+1]<<8 ); }
    int32_t chk32=0;
    for (auto it=_c.begin(); it != _c.end(); chk32 = (chk32<<1) + *it++);
    chkcalc = 0x7FFF & ((chk32 & 0x00007FFF) + (chk32 >> 15));
    
    int num=0,sum=0;
    if(d0.invalid_data){ sum+=d0.distance; num++; }
    if(d1.invalid_data){ sum+=d1.distance; num++; }
    if(d2.invalid_data){ sum+=d2.distance; num++; }
    if(d3.invalid_data){ sum+=d3.distance; num++; }

    avg_dist = double(sum)/double(num);
}


void 
packet::print() const
{
    using namespace std;
    auto val = p_time;//.time_since_epoch().count();
    printf("\nConstents of packet #%016llX",val);
    printf("\nStart byte : %02X", start);
    printf("\nIndex byte : %02X", index);
    printf("\nSpeed value: %04X", speed);
    d0.print(0);
    d1.print(1);
    d2.print(2);
    d3.print(3);
    cout << "\nChecksum: ";
    if (chkcalc==chksum) cout<<"MATCH";
    else printf("ERR %04X != %04X",chkcalc,chksum);
    cout<<endl;
    return;
}


/***********************      SCAN_MAP METHODS    ***********************/

size_t
_360_scan::add_pkt(packet* _p)
{
    using namespace packet_config;
    
    if (_p->index == index_min) { begin = _p->p_time; }
    if (_p->index == index_max) { end = _p->p_time; }

    uint degree = (_p->index - index_min) * data_num;
    //cout <<"\nAdding degree: "<< degree;
    deg_index[degree + 0]= &_p->d0 ;
    deg_index[degree + 1]= &_p->d1 ;
    deg_index[degree + 2]= &_p->d2 ;
    deg_index[degree + 3]= &_p->d3 ;
//    deg_index.emplace( pair<uint,data>(degree + 0, _p.d0) ) ;
//    deg_index.emplace( pair<uint,data>(degree + 1, _p.d1) ) ;
//    deg_index.emplace( pair<uint,data>(degree + 2, _p.d2) ) ;
//    deg_index.emplace( pair<uint,data>(degree + 3, _p.d3) ) ;
    //cout<< "\nAdding packet #" <<deg_index.size();
    //_p.print();
    return deg_index.size();
}



void
_360_scan::print_pkt()
{
	//cout << "\nPrinting...";
    for(auto it = deg_index.begin(); it != deg_index.end();it++)
    {
        if( (it->first%4 ==0)  )
            it->second->source->print();

    }
}

void
_360_scan::print_data()
{
	//cout << "\nPrinting...";
    for(auto it = deg_index.begin(); it != deg_index.end();it++)
    {
        it->second->print(it->first%4);
    }
}
