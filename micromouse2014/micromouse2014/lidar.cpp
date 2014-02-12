//
//  lidar.cpp
//  micromouse2014
//
//  Created by Lyle Moffitt on 1/21/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#include "lidar.h"

using namespace std;

template<size_t _num, size_t _wid>
bitstr<_num,_wid>::bitstr()
{
    v_0b = vector<bitset<_wid>>(_num, bitset<_wid>());
}

template<size_t _num, size_t _wid>
bitstr<_num,_wid>::bitstr(string str, int base):
v_0b(vector<bitset<_wid>>(_num, bitset<_wid>()))
{
    unsigned long long _val ( strtoull(str.c_str(), nullptr, base));
    for (size_t i = 1; i<=_num; i++) {
        v_0b.at(_num - i) = bitset<_wid>(_val);
        _val>>=_wid;
    }
}




data::data(int _d)
{
    _0b = std::bitset<32>(_d);
}

data::data(uint _d)
{
    _0b = std::bitset<32>(_d);
}

uint16_t
data::distance()
{
    return (_0b & bitset<32>(0x3FFF)).to_ulong();
}

uint16_t
data::strength()
{
    return (_0b>>16 & bitset<32>(0xFFFF)).to_ulong();
}

bool
data::strength_warn()
{
    return _0b[14];
}

bool
data::invalid()
{
    return _0b[13];
}



