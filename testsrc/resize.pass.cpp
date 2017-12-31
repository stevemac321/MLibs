/*==============================================================================
 Name        : resize.pass
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include "precompile.hpp"
#include "test_support.h"
/// Mostly testing that a vector that is reserve-like contructed behaves has
/// expected with OutputIterators.

void rp_test1()
{
	TC_BEGIN(__func__);
	{
		vector<int> v{0, 1, 2, 3, 4, 5, 6, 7, 8};
		v.resize(10);
		v.push_back(22);
		VERIFY(v == vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 22}));
	}
	{
		std::initializer_list<int> int25 = {
		    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12,
		    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
		vector<int> v;
		for (auto &i : int25)
			v.push_back(i);

		vector<int> compare(int25);
		VERIFY(v == compare);
	}
}
void rp_test2()
{
	TC_BEGIN(__func__);
	vector<int> v;
	v.reserve(1000);
	v.assign(10, 55);
	vector<int> compare{55, 55, 55, 55, 55, 55, 55, 55, 55, 55};
}

void rp_test3()
{
	TC_BEGIN(__func__);
	vector<int> dest{0, 1, 2, 3, 4, 5};
	vector<int> src{55, 66, 77};
	std::copy(src.begin(), src.end(), dest.begin());
	vector<int> compare{55, 66, 77, 3, 4, 5};
	VERIFY(dest == compare);
}
int resize_pass()
{
	rp_test1();
	rp_test2();
	rp_test3();
	return failed;
}
