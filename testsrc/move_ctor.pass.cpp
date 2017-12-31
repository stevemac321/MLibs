/*==============================================================================
 Name        : move_ctor.pass.cpp
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
int move_assign_ctor_pass()
{
	TC_BEGIN(__func__);
	Test<strpair, move, empty> pme;
	{
		vector<strpair> tomove;
		vector<strpair> x(std::move(tomove));
		VERIFY(pme.execute(tomove, x));
	}
	Test<int, move, diff> ime;
	{
		std::initializer_list<int> int5 = {0, 1, 2, 3, 4};
		vector<int> tomove(int5);
		auto cap = tomove.capacity();
		vector<int> x(std::move(tomove));
		VERIFY(ime.execute(tomove, x, cap));
	}

	{
		std::initializer_list<int> int9 = {0, 1, 2, 3, 4, 5, 6, 7, 8};
		vector<int> tomove(int9);
		auto cap = tomove.capacity();
		vector<int> x(std::move(tomove));
		VERIFY(ime.execute(tomove, x, cap));
	}

	{
		std::initializer_list<int> int25 = {
		    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12,
		    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
		vector<int> tomove(int25);
		auto cap = tomove.capacity();
		vector<int> x(std::move(tomove));
		VERIFY(ime.execute(tomove, x, cap));
	}
	// vectors that have been push_back's, resize issues
	{
		std::initializer_list<int> int5 = {0, 1, 2, 3, 4};
		vector<int> tomove;
		ilpush(tomove, int5);
		auto cap = tomove.capacity();
		vector<int> x(std::move(tomove));
		VERIFY(ime.execute(tomove, x, cap));
	}
	{
		std::initializer_list<int> int9 = {0, 1, 2, 3, 4, 5, 6, 7, 8};
		vector<int> tomove;
		ilpush(tomove, int9);
		auto cap = tomove.capacity();
		vector<int> x(std::move(tomove));
		VERIFY(ime.execute(tomove, x, cap));
	}

	{
		std::initializer_list<int> int25 = {
		    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12,
		    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
		vector<int> tomove;
		ilpush(tomove, int25);
		auto cap = tomove.capacity();
		vector<int> x(std::move(tomove));
		VERIFY(ime.execute(tomove, x, cap));
	}

	Test<std::string, move, diff> smd;
	{
		std::initializer_list<std::string> str5 = {
		    "apple", "berry", "cat", "deer", "eagle"};
		vector<std::string> tomove(str5);
		auto cap = tomove.capacity();
		vector<std::string> x(std::move(tomove));
		VERIFY(smd.execute(tomove, x, cap));
	}

	{
		std::initializer_list<std::string> str5 = {
		    "apple", "berry", "cat", "deer", "eagle"};
		vector<std::string> tomove;
		ilpush(tomove, str5);
		auto cap = tomove.capacity();
		vector<std::string> x(std::move(tomove));
		VERIFY(smd.execute(tomove, x, cap));
	}
	return failed;
}
