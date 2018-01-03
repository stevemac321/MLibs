/*==============================================================================
 Name        : main.cpp
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include "precompile.hpp"
#include "algo.hpp"
#include "allocator.hpp"
#include "functors.hpp"
#include "heap.h"
#include "iterator.hpp"
#include "test_support.h"
#include "tests.h"
#include "validate.h"
#include "vector.hpp"
#include "open_map.hpp"

#ifndef NL
#define NL printf("\n")
#endif

// algo test functions
void raw_int_array_test();
void raw_grid_array_test();
void raw_string_array_test();
void raw_record_array_test();
void priority_queue_custom_allocator_test();
void openmap_test();
void vector_test_driver();


static int a[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
static int arr[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0, -1, -2, -3};
static int grid[3][3] = {{44, 77, 11}, {2, 100, 77}, {0, 33, 9}};
static const char *strs[] = {"zero", "one", "two", "three", "four"};
static student recs[] = {{"Joe", 2.0},   {"Mary", 4.0},  {"Larry", 3.5},
			 {"Moe", 3.0},   {"Harry", 3.5}, {"Mary", 2.0},
			 {"Jill", 4.0},  {"Jerry", 2.9}, {"Mike", 3.5},
			 {"Harry", 3.0}, {"Abel", 3.5},  {"Abel", 2.0}};

// template function to print priority queue
template <typename T> void print_queue(T &q)
{
	while (!q.empty()) {
		printf("%d ", q.top());
		q.pop();
	}
	NL;
}
// Functor object to pass to for_each
struct accum {
	accum() : sum(0) {}
	void operator()(int n) { sum += n; }
	int sum;
};
// lambdas for visit
auto print_int = [](const int &i) { printf("%d ",i); };
auto print_str = [](const char *s) {
	VERIFY(s);
	printf("%s ", s);
};
auto print_student = [](const student &s) {
	printf("%s %.2f ", s.name, s.gpa);
};

// Lambdas to pass to transform
auto double_int = [](int &i) { return i * 2; };
auto rand_int = [](int &) { return rand() % 1000; };

///////////////////////////////////////////////////////////////////////////////
// main
//////////////////////////////////////////////////////////////////////////////
int main()
{
	Heap_Init();
	raw_int_array_test();
	raw_grid_array_test();
	raw_string_array_test();
	raw_record_array_test();
	priority_queue_custom_allocator_test();
	openmap_test();
	vector_test_driver();
	REPORT("x86 CPP-Algo");
}
// algo test functions
void raw_int_array_test()
{
	TC_BEGIN(__func__);
	stevemac::transform(arr, arr + _countof(arr), arr, rand_int);
	stevemac::sort(arr, arr + _countof(arr));
	stevemac::visit(arr, arr + _countof(arr), print_int);
	NL;
	auto s = stevemac::for_each(arr, arr + _countof(arr), accum());
	printf("Accum returned %d\n", s.sum);
	stevemac::transform(arr, arr + _countof(arr), arr, double_int)	;
	stevemac::visit(arr, arr + _countof(arr), print_int);
	PASSED(__func__, __LINE__);
}
void raw_grid_array_test()
{
	TC_BEGIN(__func__);
	int *pg = &grid[0][0];
	stevemac::transform(pg, pg + _gridcountof(grid), pg, rand_int);
	stevemac::sort(pg, pg + _gridcountof(grid));
	stevemac::visit(pg, pg + _gridcountof(grid), print_int);
	NL;
	auto s = stevemac::for_each(pg, pg + _gridcountof(grid), accum());
	printf("Accum returned %d\n", s.sum);
	stevemac::transform(pg, pg + _gridcountof(grid), pg, double_int);
	stevemac::visit(pg, pg + _gridcountof(grid), print_int);
	NL;
	PASSED(__func__, __LINE__);
}
void raw_string_array_test()
{
	TC_BEGIN(__func__);
	stevemac::sort(strs, strs + _countof(strs));
	stevemac::visit(strs, strs + _countof(strs), print_str);
	NL;
	PASSED(__func__, __LINE__);
}
void raw_record_array_test()
{
	TC_BEGIN(__func__);
	stevemac::sort(recs, recs + _countof(recs));
	stevemac::visit(recs, recs + _countof(recs), print_student);
	NL;
	PASSED(__func__, __LINE__);
}

void priority_queue_custom_allocator_test()
{
	TC_BEGIN(__func__);
	// stevemac::vector uses static memory custom allocator
	std::priority_queue<int, stevemac::vector<int>> q;
	for (int n : {1, 8, 5, 6, 3, 4, 0, 9, 7, 2})
		q.push(n);

	print_queue(q);

	// using std::vector with stevemac::allocator
	std::priority_queue<int, std::vector<int, stevemac::allocator<int>>, 
							std::greater<int>> q2;
	for (int n : {1, 8, 5, 6, 3, 4, 0, 9, 7, 2})
		q2.push(n);

	print_queue(q2);

	// Using lambda to compare elements.
	auto cmp = [](int left, int right) { return (left ^ 1) < (right ^ 1); };
	std::priority_queue<int, stevemac::vector<int>, decltype(cmp)> q3(cmp);

	for (int n : {1, 8, 5, 6, 3, 4, 0, 9, 7, 2})
		q3.push(n);

	print_queue(q3);

	std::vector<const char*, stevemac::allocator<const char*>> vstr {
		"hello ", "goodbye", "haulass", "moe"
	};
	for(auto s : vstr)
		printf("%s ", s);
	
	NL;
	
	stevemac::visit(vstr.begin(), vstr.end(), 
 		[](const char*& pps){ printf("%s ", pps);} );

	NL;
	stevemac::sort(vstr.data(), vstr.data() + vstr.size());
	for(auto s : vstr)
		printf("%s ", s);

	NL;
	PASSED(__func__, __LINE__);
}

// large test suite to test stevemac::vector
void vector_test_driver()
{
	algo_copy_reverse_pass();
	algo_non_mod_pass();
	clear_pass();
	copy_assign_pass();
	copy_ctor_pass();
	emplace_pass();
	erase_pass();
	init_assign_pass();
	insert_pass();
	logical_ops_pass();
	move_assign_pass();
	move_assign_ctor_pass();
	resize_pass();
	shrink_pass();
	swap_pass();
}
void openmap_test()
{
	TC_BEGIN(__func__);
	const int tablesize=97;
      	int a[tablesize];
	stevemac::transform(a, a + _countof(a), a, rand_int);
        stevemac::open_map<int, int, -7777> m(97);
	int dupes = 0;
        for(auto i : a) {
                auto p = m.insert(std::make_pair(i,i*10));
		if(p.second == false)
			dupes++;
	}

        for(auto p : m) 
        	printf("%d %d\t", p.first, p.second);
        
	NL;

	m.stats();
	printf("dupes %d\n", dupes);
	PASSED(__func__, __LINE__);
}
	
