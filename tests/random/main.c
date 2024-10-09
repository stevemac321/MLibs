#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "harness.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
#define RMAX 4096  // number of "bins"

// test helper
void dummy();
// tests
float chisquare(int N, int r); // Sedgewick, Algorithms in C++, 1992
void test_rand();

int main()
{
    PROJECT_BANNER("GRAPH Algorithms, A Fixed, Attachable Array for Embedded Enviroments");
    test_rand();
    dummy();
}
/*-----------------------------------------------------------------------------
Test functions and data
-----------------------------------------------------------------------------*/
// x^2 = Sigma 0 <=i < r (f_i - N/r^2)^2 / N/r
// see random.md in /slides or view the Doxygen project.
// Function to calculate chi-square value
float chisquare(int N, int r) {
    // Ensure the number of bins does not exceed RMAX
    if (r > RMAX) return -1.0;

    // Array to track the frequency of random values falling into each bin
    int f[RMAX] = {0};

    // Seed the random number generator for reproducibility
    srand(1234567);

    // Generate N random values and increment the frequency count in the corresponding bins
    for (int i = 0; i < N; i++) {
        f[rand() % r]++;
    }

    // Calculate the chi-square value
    float chi_square = 0.0;
    float expected_frequency = (float)N / r;

    for (int i = 0; i < r; i++) {
        float observed_frequency = (float)f[i];
        chi_square += ((observed_frequency - expected_frequency) * 
                       (observed_frequency - expected_frequency)) / expected_frequency;
    }

    return chi_square;
}
void dummy()
{
	printf("foo");
}
void test_rand()
{
	TC_BEGIN("Test random number generator");
    int N = 10000;  // Number of random samples
    int r = 16;     // Number of bins

    // Calculate the chi-square value
    float result = chisquare(N, r);

    // Print the chi-square value
    printf("Chi-Square Value: %.2f\n", result);

    // Evaluate the result to determine if the generator is uniform
    float lower_bound = r - 2 * sqrt(2 * r);
    float upper_bound = r + 2 * sqrt(2 * r);

    // Verify that the result is within the expected range
    VERIFY(result >= lower_bound && result <= upper_bound);
	REPORT("Test Random Results ");
}
#pragma GCC diagnostic pop
