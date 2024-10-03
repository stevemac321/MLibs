/** 
 * @page algo Algorithm Module
 * @brief This page provides a summary of the algorithm module.
 *
 * ## Algorithm Overview
 *
 * The `algo` module provides a set of functions for various algorithmic operations, including sorting, searching, and transformations.
 *
 * ### Key Features
 * - Function-based sorting, searching, and modifications.
 * - Custom functor support.
 * - Seamless integration with other data structures.
 *
 * ### Usage
 * ```c
 * // Example usage of the sorting function.
 * int data[] = {3, 1, 4, 1, 5, 9};
 * gensort(data, sizeof(data) / sizeof(data[0]), sizeof(int), int_less, int_swap);
 * ```
 *
 * See the detailed documentation for more information.
 */


### Algorithm Library Overview

**Purpose**:  
The `algo.c` file provides a comprehensive set of generic algorithms for manipulating and analyzing contiguous memory blocks (typically arrays). It utilizes function pointers to define behavior for various data types, making it highly adaptable and reusable for a wide range of scenarios. This library is designed to be type-agnostic, supporting operations on any element type as long as appropriate function pointers are provided for comparison, transformation, and modification operations.

### Key Features

1. **Type-Agnostic Operations**:
   - The library uses generic pointers (`genptr`) to represent contiguous memory blocks, making it independent of any specific data type.
   - Functions are parameterized with function pointers for comparison, transformation, and modification, allowing them to work with any element type.

2. **Function Pointer Customization**:
   - The library supports user-defined function pointers for:
     - Comparison (e.g., `cmp` for sorting and searching)
     - Swapping (e.g., `swp` for sort operations)
     - Transformation and modification (e.g., `func`, `mod`)
     - Predicates (e.g., `pred` for testing conditions)

3. **Algorithm Implementations**:
   - Provides a variety of algorithms for processing contiguous memory blocks, including:
     - **Sorting and Searching**: `gensort`, `gensearch`
     - **Transformation and Modification**: `visit`, `transform`, `modify`
     - **Non-modifying Algorithms**: `count_if`, `equal`, `is_sorted`, `all_of`, `any_of`, `none_of`
     - **Modifying Algorithms**: `copy`, `replace`, `replace_if`, `reverse`, `swap_ranges`
     - **Numeric Algorithms**: `accumulate`, `product`, `inner_product`

4. **Integration with Other Components**:
   - The library seamlessly integrates with other components like `functor.c` to provide specialized comparison and manipulation functions for various data types (e.g., integers, strings).
   - It also interacts with custom data structures like the `polyarray` container, enhancing the flexibility of those data structures.

### Major Functions

1. **`visit`**:
   - Iterates over a block of memory, calling the provided function pointer (`readonly`) on each element.
   - Typically used for read-only operations like printing elements.

   ```c
   void visit(genptr base, const size_t count, const size_t size, void (*readonly)(const genptr));
   ```

2. **`transform` and `modify`**:
   - `transform`: Iterates over two blocks of memory, applying a transformation function to elements in the source and storing results in the destination.
   - `modify`: Applies a modification function to each element in a single block.

   ```c
   void transform(genptr dest, const genptr src, const size_t count, const size_t size, void (*func)(const genptr, genptr));
   void modify(genptr base, const size_t count, const size_t size, void(*mod)(genptr pv));
   ```

3. **`gensort` and `gensearch`**:
   - `gensort`: Implements a generic insertion sort algorithm using the provided comparison and swap function pointers.
   - `gensearch`: Performs binary search on a sorted array.

   ```c
   void gensort(genptr base, const size_t count, const size_t size, bool (*cmp)(const genptr, const genptr), void (*swp)(genptr, genptr));
   genptr gensearch(genptr base, const genptr key, size_t first, size_t last, const size_t size, int (*cmp)(const genptr, const genptr));
   ```

4. **`count_if`, `equal`, `is_sorted`**:
   - `count_if`: Counts elements in a block that satisfy a given predicate.
   - `equal`: Compares two ranges for equality based on the comparison function.
   - `is_sorted`: Checks if the array is sorted based on a predicate.

   ```c
   size_t count_if(const genptr base, const size_t count, const size_t size, bool(*pred)(const genptr));
   bool equal(const genptr first1, const genptr first2, const size_t count, const size_t size, int(*cmp)(const genptr, const genptr));
   bool is_sorted(const genptr base, const size_t count, const size_t size, bool(*pred)(const genptr, const genptr));
   ```

5. **`copy`, `reverse_copy`, `swap_ranges`**:
   - `copy`: Copies elements from one block to another.
   - `reverse_copy`: Copies elements in reverse order.
   - `swap_ranges`: Swaps elements between two blocks.

   ```c
   void copy(genptr dest, const genptr src, const size_t count, const size_t size);
   void reverse_copy(genptr dest, const genptr src, const size_t count, const size_t size);
   void swap_ranges(genptr first1, genptr first2, const size_t count, const size_t size, void(*swap)(genptr, genptr));
   ```

6. **Numeric Algorithms**:
   - `accumulate`: Computes the sum of elements in a range.
   - `product`: Computes the product of elements in a range.
   - `inner_product`: Computes the inner product (sum of element-wise products) of two ranges.

   ```c
   int accumulate(const genptr base, const size_t count, const size_t size, int(*acc)(const genptr));
   int product(const genptr base, const size_t count, const size_t size, int(*prod)(const genptr));
   int inner_product(const genptr first1, const genptr first2, const size_t count, const size_t size, int(*prod)(const genptr));
   ```

### Usage Example

```c
int values[] = {5, 3, 7, 2, 8};
int doubled[5];

// Use `transform` to double each element in the `values` array and store results in `doubled`
transform(doubled, values, 5, sizeof(int), double_value);

// Sort the doubled array
gensort(doubled, 5, sizeof(int), int_less, int_swap);

// Print the sorted doubled array
visit(doubled, 5, sizeof(int), print_int);
```

### Summary
The `algo.c` library provides a set of powerful, type-agnostic algorithms that enable advanced manipulation and analysis of contiguous memory blocks. With its integration of function pointers, it supports flexible operations on any data type, making it a versatile solution for building complex data processing routines in C. This flexibility, combined with robust algorithm implementations, positions the library as a foundational component for data-driven applications, especially in embedded and performance-sensitive systems.