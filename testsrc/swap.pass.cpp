/*==============================================================================
 Name        : swap.pass.cpp
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include "precompile.hpp"
#include "test_support.h"
/// swap two empty for sanity check, then a conventional swap.
///  Test will run the swapped vectors through the modifying algorithm gauntlet;
///  std::reverse, std::sort, std::equality against expected results.

int swap_pass()
{
	TC_BEGIN(__func__);
	Test<int, copy, empty> ise;
	{
		vector<int> x;
		vector<int> y;
		x.swap(y);
		VERIFY(ise.execute(x, y));
	}

	Test<int, copy, diff> isd;
	{
		std::initializer_list<int> int5 = {0, 1, 2, 3, 4};
		std::initializer_list<int> int9 = {0, 1, 2, 3, 4, 5, 6, 7, 8};
		vector<int> x(int5);
		vector<int> comparex(x);
		vector<int> y(int9);
		vector<int> comparey(y);
		x.swap(y);

		VERIFY(isd.execute(x, comparey));
		VERIFY(isd.execute(y, comparex));
	}
	return failed;
}
