/**
 * @page isort Assembly Insertion Sort
 * @brief Overview of the assembly implementation of insertion sort for educational purposes.
 *
 * ## Assembly Insertion Sort Overview
 *
 * The `isort` module provides an assembly implementation of the insertion sort algorithm. This implementation is intended for educational purposes and demonstrates how to translate a C-based insertion sort algorithm into ARM assembly language. The module also includes assembly-based comparison functions that can be used with other algorithms requiring custom comparison operations.
 *
 * ### Key Features
 *
 * - **Assembly-Based Sorting**: The core `gen_sort` function implements the insertion sort algorithm entirely in ARM assembly language.
 * - **Custom Comparison Functions**: Includes assembly implementations for comparing integers using different criteria:
 *   - `asm_int_less`: Checks if one integer is less than another.
 *   - `asm_int_notless`: Checks if one integer is not less than another.
 * - **Educational Purpose**: The module serves as a learning tool for understanding how high-level C algorithms can be translated into low-level assembly language.
 *
 * ### Major Functions
 *
 * - **`gen_sort`**:
 *   - Implements the insertion sort algorithm in assembly language.
 *   - Parameters:
 *     - `int *a`: Pointer to an array of integers to sort.
 *     - `const size_t count`: Number of elements in the array.
 *     - `int (*pred)(const int, const int)`: Function pointer for comparing two integers.
 *
 * - **Comparison Functions**:
 *   - `asm_int_less`: Checks if one integer is less than another. Returns 1 if true, 0 otherwise.
 *   - `asm_int_notless`: Checks if one integer is not less than another. Returns 1 if true, 0 otherwise.
 *
 * ### Function Descriptions
 *
 * - **`asm_int_less`**:
 *   - Compares two integers.
 *   - Returns 1 if the first integer is less than the second.
 *
 * - **`asm_int_notless`**:
 *   - Compares two integers.
 *   - Returns 1 if the first integer is not less than the second.
 *
 * - **`gen_sort`**:
 *   - Implements the insertion sort algorithm for an array of integers.
 *   - The outer loop iterates through each element starting at index 1.
 *   - The inner loop compares each element with its predecessor and swaps them if necessary until the element reaches its proper position in the sorted partition.
 *
 * ### Usage Example
 *
 * ```assembly
 * // Assembly snippet to call gen_sort with a comparison function
 * mov r0, #0       // Address of the array
 * mov r1, #10      // Number of elements
 * mov r2, asm_int_less  // Comparison function
 * bl gen_sort      // Call the sorting function
 * ```
 *
 * ### Assembly Implementation Details
 *
 * - The `gen_sort` function uses several ARM assembly instructions such as `ldr`, `cmp`, and `blx` to load, compare, and branch to the appropriate functions.
 * - Registers `r0`, `r1`, `r2`, etc., are used to hold function parameters and results.
 * - The stack is used to store context information before branching to function calls.
 * - `ldr` and `str` instructions are used to load and store elements from the array.
 *
 * ### Summary
 * The `isort` module provides an assembly-based implementation of the insertion sort algorithm, serving as a bridge between high-level algorithms and low-level assembly programming. It showcases the translation of C code into ARM assembly and includes custom comparison functions to demonstrate the flexibility of assembly-based algorithms.
 */

### Assembly Insertion Sort: `gen_sort`

**Purpose**:  
The `gen_sort` function is an assembly-implemented insertion sort designed for educational purposes. It serves as an example of how sorting algorithms can be implemented directly in ARM assembly, utilizing the `Thumb` instruction set. This implementation demonstrates key concepts of low-level programming and the use of function pointers in assembly.

### Key Features

1. **Insertion Sort Implementation**:  
   The `gen_sort` function uses the classic insertion sort algorithm. It iterates through the array, comparing each element with the elements in the sorted portion of the array. If an element is smaller than the elements before it, it is swapped until it reaches its correct position in the sorted portion.

2. **Generic Sorting Function**:  
   `gen_sort` is designed to work with any contiguous block of memory that holds elements of the same size as a pointer. It can be used to sort various data types such as integers, pointers, and custom structures.

3. **User-Defined Comparison Function**:  
   The function uses a user-supplied comparison function, passed as a function pointer. This allows `gen_sort` to work with different data types and sorting criteria, making it a highly flexible assembly routine.

4. **Supporting Assembly Functions**:
   - **`asm_int_less`**: A basic comparison function for integers, returning `1` if the first argument is less than the second, and `0` otherwise.
   - **`asm_int_notless`**: A basic comparison function for integers, returning `1` if the first argument is not less than the second, and `0` otherwise.

### Assembly Implementation Details

1. **Register Usage**:
   - **`r0`**: Base address of the array.
   - **`r1`**: Number of elements in the array.
   - **`r2`**: Pointer to the comparison function.
   - **`r7`**: Outer loop counter.
   - **`r8`**: Inner loop counter, used for element comparisons.
   - **`r9`**: Inner loop index (`current - 1`).
   - **`r10`**: Temporary register for comparison result.
   - **`r11`**: Temporary register for storing the comparison function pointer.

2. **Algorithm Flow**:
   - The outer loop (`for_loop`) iterates through each element starting from the second element.
   - The inner loop (`while_loop`) iterates backward through the sorted portion of the array, swapping elements as needed to place the `current` element in its correct position.
   - The comparison function is called within the inner loop to determine the correct position of the element.

3. **Usage of Function Pointers in Assembly**:
   The function pointer for comparison is passed to `gen_sort` via register `r2`. It is used in the `blx` instruction to branch to the comparison function dynamically. This demonstrates how function pointers can be utilized in low-level assembly code to implement generic algorithms.

### Example C Usage

This assembly function can be called from C code like this:

```c
// Example C function to use with `gen_sort`
int compare_ints(int a, int b) {
    return a < b;
}

int main() {
    int array[] = {5, 2, 9, 1, 5, 6};
    size_t size = sizeof(array) / sizeof(array[0]);

    // Call assembly insertion sort
    gen_sort(array, size, compare_ints);

    for (int i = 0; i < size; ++i) {
        printf("%d ", array[i]);
    }
    return 0;
}
```

### Summary
The `gen_sort` assembly function provides a low-level implementation of the insertion sort algorithm, demonstrating how sorting can be achieved in assembly language. The use of function pointers for comparison functions makes it a flexible and reusable routine. This function is a good example of integrating assembly code with C and leveraging ARM-specific features for educational and performance-critical applications.