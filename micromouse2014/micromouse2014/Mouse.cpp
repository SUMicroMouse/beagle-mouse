#include "Mouse.h"

namespace Algorithm
{
	Mouse::Mouse() :
		// member-initializer list
		maze(new Data::Maze()),
		sensor(new Hardware::Sensor())
	{
		location = maze->Start();
		maze->printMaze(false);
	}

	Mouse::~Mouse()
	{
		//delete maze;
		//delete sensor;
	}

	void Mouse::Evaluate()
	{
		// Get information from sensors
		auto senseGrid = this->sensor->read();

		// Update the cells' heuristic values starting from the farthest goal cell
		Searches::breadth_first_search(*maze, 
				8,//
				8);	////
			//	/	//	/	/	//	///	//	/

		// Generate Paths
		// Filter/Choose Path
		auto path_selected = GeneratePaths();


		// Go to the next position





		int i = 0;
	}

	std::unique_ptr<Path_new> * Mouse::GeneratePaths()
	{
		// Generate paths
		std::deque<Path_new*> *ps = Searches::depth_search(*maze, 
															*location, 
															*new Location(8, 8));// 
		// / / /// ///
		//
		///
		//////////
		///
		//
		std::deque<std::unique_ptr<Path_new>> paths(ps->begin(), ps->end());

		// Filter paths
		Filter::mode = ExploreMode::Direct;
		std::unique_ptr<Path_new>* path_selected = Filter::bestPath(&paths);

		return path_selected;
	}
}