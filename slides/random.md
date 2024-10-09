/**
 * @page random Random Number Testing Module
 * @brief This page provides an overview of the random number testing module.
 *
 * ## Random Number Testing Overview
 *
 * The `random` test module is designed to evaluate the quality of random number generation
 * in embedded systems using a chi-square test. This test checks whether the distribution of
 * randomly generated numbers fits the expected uniform distribution. It provides insight into
 * the randomness and uniformity of the pseudo-random number generator being used.
 *
 * ### Key Features
 * - **Chi-Square Test**: Verifies the randomness of the distribution by comparing the observed
 *   frequencies of numbers to the expected frequencies.
 * - **Flexible Configuration**: Supports variable sample sizes (`N`) and number of bins (`r`) to 
 *   accommodate different distributions and use cases.
 * - **Comprehensive Output**: Outputs chi-square test results, helping to assess the quality of
 *   the random number generator.
 *
* See the detailed documentation for more information.
 */
 * ### Usage
 * ```c
 * int sample_size = 10000;  // Number of random values to generate
 * int bins = 16;            // Number of bins for the chi-square test
 * float chi_square_result = chisquare(sample_size, bins);
 * ```
 *
 * The `chisquare` function generates `sample_size` random numbers and counts their occurrence
 * in `bins` number of bins. The resulting chi-square value is compared against a threshold to
 * evaluate the uniformity of the random number generator.
 *
 * ### Major Functions
 *
 * - **chisquare**: Calculates the chi-square value for a given set of random samples.
 *   ```c
 *   float chisquare(int N, int r);
 *   ```
 *   - **Parameters**:
 *     - `N`: Number of random values to generate and evaluate.
 *     - `r`: Number of bins to use for calculating the chi-square value.
 *   - **Returns**: The calculated chi-square value. A value close to `r` indicates a good fit for
 *     a uniform distribution, whereas larger deviations suggest non-uniformity.
 *
 * - **test_rand**: Performs a series of tests on the random number generator to validate its
 *   behavior and outputs the results.
 *
 * ### Example Code
 * ```c
 * // Perform a chi-square test with 10,000 samples and 16 bins
 * float result = chisquare(10000, 16);
 * if (fabs(result - 16) < 2 * sqrt(2 * 16)) {
 *     printf("The random number generator appears to be uniform.\n");
 * } else {
 *     printf("The random number generator does not appear to be uniform.\n");
 * }
 * ```
 *
 * ### Output
 * The module outputs the calculated chi-square value along with a pass/fail message indicating
 * whether the random number generator passes the uniformity test.
 *
 * See the detailed function documentation for more information.
 */
