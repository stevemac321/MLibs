/*==============================================================================
 Name        : copy_ctor.pass.cpp
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include "precompile.hpp"
#include "test_support.h"
/// Testing the matrix: the template arguments depict the scenario:
/// first parameter is T, second is copy or move, third argument
/// describes the copy\move source, empty or different (full).
/// Note we are testing std::string as T also.
using strpair = std::pair<std::string, std::string>;
int copy_ctor_pass()
{
	TC_BEGIN(__func__);
	Test<int, copy, empty> ice;
	VERIFY(ice.execute(vector<int>(), vector<int>()));

	Test<strpair, copy, empty> pce;
	VERIFY(pce.execute(vector<strpair>(), vector<strpair>()));

	Test<int, copy, diff> icd;
	{
		std::initializer_list<int> int5 = {0, 1, 2, 3, 4};
		vector<int> x(int5);
		vector<int> y(x);
		VERIFY(icd.execute(x, y));
	}
	{
		std::initializer_list<int> int9 = {0, 1, 2, 3, 4, 5, 6, 7, 8};
		vector<int> x(int9);
		vector<int> y(x);
		VERIFY(icd.execute(x, y));
	}
	{
		std::initializer_list<int> int25 = {
		    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12,
		    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
		vector<int> x(int25);
		vector<int> y(x);
		VERIFY(icd.execute(x, y));
	}
	// push_backed vectors can have larger capacities
	{
		std::initializer_list<int> int5 = {0, 1, 2, 3, 4};
		vector<int> x;
		ilpush(x, int5);
		vector<int> y(x);
		VERIFY(icd.execute(x, y));
	}
	{
		std::initializer_list<int> int9 = {0, 1, 2, 3, 4, 5, 6, 7, 8};
		vector<int> x;
		ilpush(x, int9);
		vector<int> y(x);
		VERIFY(icd.execute(x, y));
	}
	{
		std::initializer_list<int> int25 = {
		    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12,
		    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
		vector<int> x;
		ilpush(x, int25);
		vector<int> y(x);
		VERIFY(icd.execute(x, y));
	}

	/// test for vector(size_type n, const T& val, const Allocator
	{
		vector<int> x(10, 66);
		vector<int> compare{66, 66, 66, 66, 66, 66, 66, 66, 66, 66};
		VERIFY(x == compare);
	}
	{
		vector<int> x(0, 66);
		VERIFY(x == vector<int>());
	}
	{
		std::initializer_list<int> int5 = {0, 1, 2, 3, 4};
		vector<int> x(5, 55);
		ilpush(x, int5);
		vector<int> y({55, 55, 55, 55, 55, 0, 1, 2, 3, 4});
		VERIFY(icd.execute(x, y));
	}
	/// test for range ctor
	{
		vector<int> x{0, 1, 2, 3, 4};
		vector<int> y(x.begin() + 1, x.begin() + 3);
		vector<int> compare{1, 2};
		VERIFY(icd.execute(y, compare));
	}
	Test<std::string, copy, diff> scd;
	{
		std::initializer_list<std::string> str5 = {
		    "apple", "berry", "cat", "deer", "eagle"};
		vector<std::string> x(str5);
		vector<std::string> y(x);
		VERIFY(scd.execute(x, y));
	}
	{
		std::initializer_list<std::string> str5 = {
		    "apple", "berry", "cat", "deer", "eagle"};
		vector<std::string> x;
		ilpush(x, str5);
		vector<std::string> y(x);
		VERIFY(scd.execute(x, y));
	}
	return failed;
}
