/*==============================================================================
 Name        : shrink.pass
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include "precompile.hpp"
#include "test_support.h"
/// NOTE: explicitly testing for capacity only works for stevemac::vector;
/// hence the conditional block when testing std::vector.
int shrink_pass()
{
	TC_BEGIN(__func__);
	vector<int> v;
	// push_back from nothing will give us cap headroom
	for (int i = 0; i < 100; i++)
		v.push_back(i);

	VERIFY(v.size() == 100);
	VERIFY(v.capacity() == 160);
	
	v.shrink_to_fit();

	VERIFY(v.size() == 100);
	VERIFY(v.capacity() == 100);
	VERIFY(v.front() == 0);
	VERIFY(v.back() == 99);
	VERIFY(reverse_sort_compare(v, v));
	return failed;
}
