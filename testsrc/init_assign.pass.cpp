/*==============================================================================
 Name        : init_assign.pass.cpp
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include "precompile.hpp"
#include "test_support.h"
/// Test copy assign initializer_list

void iap_test1()
{
	TC_BEGIN(__func__);
	vector<int> v{0, 1, 2, 3, 4};
	v = {5, 5, 6, 7};
	vector<int> compare{5, 5, 6, 7};
	VERIFY(v == compare);
}
int init_assign_pass()
{
	iap_test1();
	return failed;
}
