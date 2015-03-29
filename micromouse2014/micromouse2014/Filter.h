#ifndef __FILTERS_H__
#define __FILTERS_H__

#include <deque>
#include <memory>
#include "ExploreMode.h"
#include "Path_new.h"
#include "Room.h"
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

		/*
		Return pointer to the room with the least visits in a given collection
		*/
		template <typename SequenceContainer>
		static Data::Room * LeastVisited(SequenceContainer collection)
		{
			if (collection->size() == 0)
				return nullptr;

			int equalCount = 1;

			Data::Room * lowest = collection->front();
			for (int i = 1; i < collection->size(); i++)
			{
				Data::Room * cu = collection->at(i);
				if (cu->visits < lowest->visits)
					lowest = cu;
				else if (cu->visits == lowest->visits)
					equalCount++;
			}

			return lowest;
		}
	};	
}
#endif