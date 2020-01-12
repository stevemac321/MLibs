#include "precompile.hpp"
#include "heap.h"
#include "functors.hpp"
#include "algo.hpp"
#include <stm32f4xx_usart.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

#ifndef NL
#define NL printf("\n")
#endif

// algo test functions
void raw_int_array_test();
void raw_grid_array_test();
void raw_string_array_test();
void raw_record_array_test();
void priority_queue_custom_allocator_test();

// test data
// array interface tests
static int a[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

static int arr[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0, -1, -2, -3};
static int grid[3][3] = {{44, 77, 11}, {2, 100, 77}, {0, 33, 9}};
static const char *strs[] = {"zero", "one", "two", "three", "four"};
static student recs[] = {{"Joe", 2.0},   {"Mary", 4.0},  {"Larry", 3.5},
			 {"Moe", 3.0},   {"Harry", 3.5}, {"Mary", 2.0},
			 {"Jill", 4.0},  {"Jerry", 2.9}, {"Mike", 3.5},
			 {"Harry", 3.0}, {"Abel", 3.5},  {"Abel", 2.0}};

// Functor object to pass to for_each
struct accum {
	accum() : sum(0) {}
	void operator()(int n) { sum += n; }
	int sum;
};
// lambdas for visit
auto print_int = [](const int &i) { printf("%d ", i); };

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

inline void Delay(const size_t duration=2000000)
{
	for (int i = 0; i < duration; i++);
}
void dummy();
int main()
{
	Heap_Init();
	raw_int_array_test();
	raw_grid_array_test();
	raw_string_array_test();
	raw_record_array_test();
	REPORT("emb cpp-test");
	dummy();

	for (;;) {
		printf("LOOPING\n");
		Delay();
		
	}
}
void dummy()
{
	printf("killing app");
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
	printf("accum returned %d\n", s.sum);
	stevemac::transform(arr, arr + _countof(arr), arr, double_int);
	stevemac::visit(arr, arr + _countof(arr), print_int);
	NL;
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
	printf("accum returned %d\n", s.sum);
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
void PrintByte(char c)
{
  USART_SendData(USART2, c);
  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
          ; /* do nothing until RESET state*/
}

size_t __write(int handle, const unsigned char* data, size_t byteCount)
{
    size_t nChars = 0;

    // handle == -1 means "flush all streams". Not implemented here.
    if (handle == -1 || byteCount == 0)
        return 0;

    // 1 - stdout; 2 - stderr. 
    if (handle != 1 && handle != 2)
        return -1;

    while (byteCount-- > 0) {
        char ch = (char)(*data);
        PrintByte(ch);
        if (ch == '\n') {
            PrintByte('\r');
        }
       ++data;
       ++nChars;
    }
    return nChars;

} // __write
#pragma GCC diagnostic pop
