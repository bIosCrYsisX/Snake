///////////////////////////////////////////////////////////////////////
// Workfile: Main.cpp
// Author: Christoph Dalpiaz
// Date: 24.07.2020
// Description: test driver for Snake module
// Remarks: -
// Revision: 0
///////////////////////////////////////////////////////////////////////

#include <iostream>	//include IO library
#include "Snake.h"  //include snake module

using namespace std;	//resolve std namespace

int main()
{
	try {
		Snake snake{};

		snake.Play();
	}
	catch (std::bad_alloc const& ex) {
		cerr << ex.what() << endl;
		return 1;
	}
	catch (std::exception const& ex) {
		cerr << ex.what() << endl;
		return 1;
	}
	catch (...) {
		cerr << "Unhandled exception!" << endl;
	}
	return 0;
}