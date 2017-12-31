/*==============================================================================
 Name        : insert.pass.cpp
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include "precompile.hpp"
#include "test_support.h"
/// insert semantics required writing a lot of code, there are a lot of
/// overloads. Insert can cause a resize (reallocation), or can be performed
/// inplace.  These tests do both and are constructed in different ways.
/// After the insert is performed, further stablity and correctness is performed
/// with algorithms, passing the insert affected vector to std::reverse,
/// std::sort, then compared with an expected range using std::equal.

void ip_test1()
{
	TC_BEGIN(__func__);
	vector<int> v;
	for (int i = 0; i < 11; i++)
		v.push_back(i);

	v.insert(v.begin() + 3, 55);
	VERIFY(v[3] == 55);
	VERIFY(v == vector<int>({0, 1, 2, 55, 3, 4, 5, 6, 7, 8, 9, 10}));
	VERIFY(v.size() == 12);
	VERIFY(reverse_sort_compare(v, v));
}

void ip_test2()
{
	TC_BEGIN(__func__);
	vector<int> v{0, 1, 2};
	v.insert(v.begin() + 1, 55);
	VERIFY(v == vector<int>({0, 55, 1, 2}));
	VERIFY(v.size() == 4);
	VERIFY(reverse_sort_compare(v, v));
}
void ip_test3()
{
	TC_BEGIN(__func__);
	vector<int> v{0};
	v.insert(v.begin(), 55);

	VERIFY(v == vector<int>({55, 0}));
	VERIFY(v.size() == 2);
	VERIFY(reverse_sort_compare(v, v));
}
void ip_test4()
{
	TC_BEGIN(__func__);
	vector<int> v;
	v.insert(v.begin(), 55);

	VERIFY(v == vector<int>({55}));
	VERIFY(v.size() == 1);
}
void ip_test5()
{
	TC_BEGIN(__func__);
	vector<int> v;
	for (int i = 0; i < 11; i++)
		v.push_back(i);
	int x = 55;
	v.insert(v.begin() + 3, x);

	VERIFY(v[3] == 55);
	VERIFY(v == vector<int>({0, 1, 2, 55, 3, 4, 5, 6, 7, 8, 9, 10}));
	VERIFY(v.size() == 12);
	VERIFY(reverse_sort_compare(v, v));
}
void ip_test6()
{
	TC_BEGIN(__func__);
	vector<int> v{0, 1, 2};
	v.insert(v.begin() + 1, 10, 55);
	VERIFY(v ==
	       vector<int>({0, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 1, 2}));
	VERIFY(reverse_sort_compare(v, v));
}
void ip_test7()
{
	TC_BEGIN(__func__);
	vector<int> x{0, 1, 2, 3, 4};
	x.insert(x.begin() + 1, {55, 55});
	vector<int> compare{0, 55, 55, 1, 2, 3, 4};
	VERIFY(x == compare);
}
void ip_test8()
{
	TC_BEGIN(__func__);
	vector<int> x{55, 55, 55, 55, 55};
	vector<int> y{0, 1, 2, 3, 4};
	y.insert(y.begin() + 1, x.begin() + 1, x.begin() + 3);
}
void ip_test9()
{
	TC_BEGIN(__func__);
	vector<int> x{0, 1, 2, 3, 4, 5};
	vector<int> y(10);
	std::copy(x.begin(), x.end(), std::back_inserter(y));
	vector<int> compare{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5};
	VERIFY(y == compare);
}

int insert_pass()
{
	ip_test1();
	ip_test2();
	ip_test3();
	ip_test4();
	ip_test5();
	ip_test6();
	ip_test7();
	ip_test8();
	ip_test9();
	return failed;
}
