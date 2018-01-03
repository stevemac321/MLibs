/*==============================================================================
 Name        : main.cpp
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include "precompile.hpp"
#include "functor.h"
#include "algo.h"

#ifndef NL
#define NL printf("\n")
#endif

template <typename T, size_t N>
class Array {
};
template <size_t N>
class Array<int, N> {
public:
	Array()=default;

	void sort()
	{
		gensort(array, N, sizeof(int), int_genless, int_genswap);
	}
	void print()
	{
		for(int i=0; i < N; i++)
			printf("%d ", *(array + i));

		NL;
	}
	int get(const size_t pos) const
	{
		return *(array + pos);
	}
	void set(const int& val, const size_t pos)
	{
		*(array + pos) = val;
	}
private:
	int array[N] = {0};
};
template <size_t N>
class Array<student, N> {
public:
	Array()=default;

	void sort()
	{
		gensort(array, N, sizeof(student), record_genless, record_genswap);
	}
	void print()
	{
		for(int i=0; i < N; i++)
			printf("%s %.02f ", array[i].name, array[i].gpa);

		NL;
	}
	student&  get(const size_t pos) const
	{
		return *(array + pos);
	}
	void set(const student& val, const size_t pos)
	{
		*(array + pos) = val;
	}
private:
	student array[N] = {0};
};

// test functions
void test_intarray();
void test_grid();
void test_recordarray();
///////////////////////////////////////////////////////////////////////////////
// main
//////////////////////////////////////////////////////////////////////////////
int main()
{
        test_intarray();
	test_grid();
	test_recordarray();
	REPORT("x86 mixed");
}
void test_intarray()
{
	TC_BEGIN(__func__);
	static int a[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};	
	Array<int,10> ar;

	for(int i=0; i < 10; i++)
		ar.set(a[i], i);
	
	ar.print();
	ar.sort();
	ar.print();
	PASSED(__func__, __LINE__);
}

void test_grid()
{
	TC_BEGIN(__func__);
	static int grid[3][3] = {{44, 77, 11}, {2, 100, 77}, {0, 33, 9}};
	Array<int, 9> ar;
	int pos = 0;
	for(int row = 0; row < 3; row++)
		for(int col=0; col < 3; col++)
			ar.set(grid[row][col], pos++ );

	ar.print();
	ar.sort();
	ar.print();

	PASSED(__func__, __LINE__);	
}

void test_recordarray()
{
	TC_BEGIN(__func__);
	static student recs[] = {{"Joe", 2.0},   {"Mary", 4.0},  {"Larry", 3.5},
			 {"Moe", 3.0},   {"Harry", 3.5}, {"Mary", 2.0},
			 {"Jill", 4.0},  {"Jerry", 2.9}, {"Mike", 3.5},
			 {"Harry", 3.0}, {"Abel", 3.5},  {"Abel", 2.0}};

	Array<student, 12> ar;
	for(int i=0; i < 12; i++)
		ar.set(recs[i], i);

	ar.print();
	ar.sort();
	ar.print();

	PASSED(__func__, __LINE__);	
}
