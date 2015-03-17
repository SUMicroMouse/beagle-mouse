#include "Filter.h"

namespace Algorithm
{
	ExploreMode Filter::mode = ExploreMode::None;

	Path_new * bestPath(std::deque<Path_new*> *paths)
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

		Sorting::Sorts<Path_new*>::quicksort(*paths, 0, paths->size());

		Path_new * returnedPath = paths->front();

		paths->clear();

		assert(returnedPath != nullptr);



		return returnedPath;
	}
}