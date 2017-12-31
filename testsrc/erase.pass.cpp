/*==============================================================================
 Name        : erase.pass.cpp
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include "precompile.hpp"
#include "remove_erase.h"
#include "test_support.h"

using namespace remove_erase::details;

void test1()
{
	TC_BEGIN(__func__);
	vector<int> v{0, 1, 2, 3, 4};
	auto after = v.erase(v.begin() + 2);
	VERIFY(v.size() == 4);
	VERIFY(v.capacity() == 5);
	VERIFY(*after == 3);
}

void test2()
{
	TC_BEGIN(__func__);
	vector<int> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	auto after = v.erase(v.begin() + 2, v.begin() + 4);
	VERIFY(v.size() == 8);
	VERIFY(v[2] == 4);
	VERIFY(reverse_sort_compare(v, v));
}
/// see comment above, this exposes a bug somewhere building with vc-stl.
///
void test3()
{
	TC_BEGIN(__func__);
	vector<int> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	v.erase(std::remove_if(v.begin(), v.end(), [](int i) { return i % 2; }),
		v.end());

	VERIFY(reverse_sort_compare(v, vector<int>({0, 2, 4, 6, 8})));

	vector<int> x{0, 1, 2, 3, 4, 5};
	vector<int> y{0, 1, 2, 3, 4, 5};
	VERIFY(x == y);
}
int erase_pass()
{
	test1();
	test2();
	test3();
	return failed;
}
