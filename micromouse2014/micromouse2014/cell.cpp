//
//  cell.cpp
//  micromouse2014
//
//  Created by Lyle Moffitt on 1/21/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#include "cell.h"

using namespace std;

cell::cell()
{
    adj_east = adj_north = adj_south = adj_west = nullptr;
}