#ifndef __FILTERS_H__
#define __FILTERS_H__

#include <deque>
#include <memory>
#include "Path_new.h"
#include "ExploreMode.h"
#include "Sorts.h"

namespace Algorithm
{
	class Filter
	{
	public:
		static ExploreMode mode;
		static std::unique_ptr<Path_new> * bestPath(std::deque<std::unique_ptr<Path_new>> *paths)
		{
			switch (Filter::mode)
			{
			case ExploreMode::FarReaching:
				Path_new::mode = ExploreMode::FarReaching;
				break;
			case ExploreMode::Direct:
				Path_new::mode = ExploreMode::Direct;
				break;
			default:
				return nullptr;
			}

			//Sorting::Sorts<std::unique_ptr<Path_new>>::quicksort(*paths, 0, paths->size()-1);
			
			// Merge sort iteratively
			Sorting::SortingT<std::deque<std::unique_ptr<Path_new>>>::mergesort_i(*paths);

			std::unique_ptr<Path_new> & returnedPath = paths->front();

			return &returnedPath;
		}
	};	
}
#endif