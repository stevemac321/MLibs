/*==============================================================================
 Name        : emplace.pass.cpp
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include "precompile.hpp"
#include "test_support.h"
/// emplace_back and insert emplace
///
void ep_test1()
{
	TC_BEGIN(__func__);
	vector<int> v;
	for (int i = 0; i < 10; i++)
		v.emplace_back(i);

	vector<int> compare{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	VERIFY(v == compare);
}

int emplace_pass()
{
	ep_test1();
	return failed;
}
