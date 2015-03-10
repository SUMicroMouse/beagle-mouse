#ifndef __FILTERS_H__
#define __FILTERS_H__

#include <deque>
#include "Path_new.h"
#include "ExploreMode.h"
#include "Sorts.h"

namespace Algorithm
{
	class Filter
	{
	public:
		static ExploreMode mode;
		static Path_new * bestPath(std::deque<Path_new*> *paths);
	};	
}
#endif