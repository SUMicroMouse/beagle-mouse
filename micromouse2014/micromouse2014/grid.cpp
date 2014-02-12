//
//  grid.cpp
//  micromouse2014
//
//  Created by Lyle Moffitt on 1/21/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#include "grid.h"

using namespace std;

grid::grid():
axis_x(*new vector<vector<cell*>>(1,vector<cell*>(1, nullptr))),
axis_y(*new vector<vector<cell*>>(1,vector<cell*>(1, nullptr)))
{
    origin = goal = center = nullptr;
}