/* array size macros */
#ifndef _countof
#define _countof(x) sizeof(x) / sizeof(x[0])
#endif

#ifndef _gridcountof
#define _gridcountof(x) sizeof(x) / sizeof(x[0][0])
#endif

typedef void *genptr;

/* TEST HARNESS*/
extern int passed;
extern int failed;
extern int tcs;
#define TC_BEGIN(func) \
	printf("--------------------TESTCASE BEGIN %s ---------------------\n", func); \
	tcs++;
#define VERIFY(x) \
	(x) ? passed++ : failed++; \
	assert(x)
#define PASSED(func, line) \
	printf("--------------------PASSED %s Line: %d --------------------\n", func, line); \
	passed++;
#define REPORT(msg) \
	printf("--%s TESTCASES: %d TESTS PASSED: %d TESTS FAILED: %d-------\n", msg, tcs, passed, failed);

