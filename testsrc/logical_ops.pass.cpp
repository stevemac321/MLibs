/*==============================================================================
 Name        : logical_ops.pass.cpp
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include "precompile.hpp"
#include "test_support.h"
/// just sanity checks, I just used the libcxx logic.
///
void lop_test1()
{
	TC_BEGIN(__func__);
	vector<int> x{5, 6, 7, 8};
	vector<int> y{5, 6, 7, 9};

	VERIFY(x < y);
	VERIFY(x != y);
	VERIFY(y > x);
	VERIFY(x <= y);
	VERIFY(y >= x);
}

int logical_ops_pass()
{
	lop_test1();
	return failed;
}
