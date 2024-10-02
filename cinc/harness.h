/* array size macros */


#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <string.h>

// Define a structure to hold test statistics
typedef struct {
    int passed;
    int failed;
    int tcs; // Test case count
} TestStats;

// Initialize a global instance of TestStats
TestStats stats = {0, 0, 0};

/* TEST HARNESS */
#define TC_BEGIN(func) \
    do { \
        printf("-------------------- TESTCASE BEGIN: %s ---------------------\n", func); \
        stats.tcs++; \
    } while (0)

#define VERIFY(x) \
    do { \
        if (x) { \
            stats.passed++; \
        } else { \
            stats.failed++; \
            printf("TEST FAILED at %s:%d - Expression: %s\n", __FILE__, __LINE__, #x); \
        } \
        assert(x); \
    } while (0)

#define PASSED(func, line) \
    do { \
        printf("---- PASSED: %s at Line: %d ----\n", func, line); \
        stats.passed++; \
    } while (0)

#define FAILED(func, line) \
    do { \
        printf("---- FAILED: %s at Line: %d ----\n", func, line); \
        stats.failed++; \
    } while (0)

#define REPORT(msg) \
    do { \
        time_t now = time(NULL); \
        char *timestamp = ctime(&now); \
        timestamp[strlen(timestamp) - 1] = '\0'; /* Remove newline */ \
        printf("-- %s | TESTCASES: %d | TESTS PASSED: %d | TESTS FAILED: %d | Timestamp: %s --\n", \
               msg, stats.tcs, stats.passed, stats.failed, timestamp); \
    } while (0)

#define RESET() \
    do { \
        stats.passed = 0; \
        stats.failed = 0; \
        stats.tcs = 0; \
    } while (0)
