/**
 * @page polyarray Polyarray Dynamic Array Implementation
 * @brief Overview of the Polyarray dynamic array implementation for the MLibs project.
 *
 * ## Polyarray Dynamic Array Overview
 *
 * The `polyarray` container provides a flexible and type-agnostic dynamic array implementation with support for custom operations and memory management. This container leverages a custom static heap manager and function pointers to enable various element operations like comparison, swapping, and printing.
 *
 * ### Key Features
 *
 * - **Dynamic Sizing and Allocation**:
 *   - The `polyarray` supports dynamic memory allocation using a custom static heap manager, which allows for more efficient memory usage in constrained environments.
 *   - The container can grow as needed and is well-suited for applications requiring dynamic data structures.
 *
 * - **Custom Element Operations**:
 *   - Function pointers for comparison (`cmp`), less-than operation (`lessthan`), swapping (`swap`), and printing (`print`) provide flexibility for custom element types.
 *   - This design allows for seamless integration with various data types and custom operations.
 *
 * - **Encapsulation and Opaque Interface**:
 *   - The container utilizes an opaque interface (`polyptr`) to abstract implementation details and enforce encapsulation.
 *
 * - **Generic Algorithms Integration**:
 *   - The `polyarray` integrates with a set of generic algorithms, allowing operations like sorting, searching, and transformations to be applied.
 *
 * ### Major Functions
 *
 * - **`poly_alloc`**:
 *   - Allocates and initializes a `polyarray` with a specified capacity and element size.
 *   - Allows the user to define custom comparison, less-than, swap, and print functions.
 *   - Function Signature:
 *     ```c
 *     polyptr poly_alloc(const size_t capacity, const size_t datasize,
 *                        int (*cmp)(const genptr, const genptr),
 *                        bool(*lessthan)(const genptr, const genptr),
 *                        void(*swap)(genptr, genptr),
 *                        void(*print)(const genptr));
 *     ```
 *
 * - **`poly_add`**:
 *   - Adds a specified number of elements to the array.
 *   - Ensures the added data does not exceed the capacity of the array.
 *   - Function Signature:
 *     ```c
 *     void poly_add(polyptr pa, const genptr data, const size_t count);
 *     ```
 *
 * - **`poly_at` and `poly_set`**:
 *   - `poly_at`: Provides access to an element at a given position.
 *   - `poly_set`: Sets the value of an element at a specified position.
 *   - Function Signature:
 *     ```c
 *     iterator poly_at(polyptr pa, const size_t pos);
 *     void poly_set(polyptr pa, const genptr val, const size_t pos);
 *     ```
 *
 * - **`poly_sort` and `poly_search`**:
 *   - `poly_sort`: Sorts the elements in the array using a provided sorting algorithm.
 *   - `poly_search`: Searches for an element in a sorted array.
 *   - Function Signature:
 *     ```c
 *     void poly_sort(polyptr pa);
 *     genptr poly_search(const polyptr pa, const genptr val);
 *     ```
 *
 * - **`poly_free`**:
 *   - Frees the allocated memory for the array and its elements.
 *   - Function Signature:
 *     ```c
 *     void poly_free(polyptr pa);
 *     ```
 *
 * ### Usage Example
 *
 * ```c
 * // Allocate a polyarray for integers with a capacity of 100 elements.
 * polyptr int_array = poly_alloc(100, sizeof(int), int_cmp, int_less, int_swap, print_int);
 *
 * // Add elements to the array.
 * int values[] = {5, 3, 7, 2, 8};
 * poly_add(int_array, values, 5);
 *
 * // Sort and print the array.
 * poly_sort(int_array);
 * poly_print(int_array);
 *
 * // Free the allocated memory.
 * poly_free(int_array);
 * ```
 *
 * ### Summary
 *
 * The `polyarray` container provides a versatile and powerful dynamic array solution that is highly customizable through the use of function pointers and custom memory management. It is well-suited for environments with strict memory constraints and scenarios requiring encapsulated, dynamic data structures. Its integration with generic algorithms makes it a valuable tool for implementing flexible and type-agnostic arrays.
 */

### Polyarray Container Overview

**Purpose**:  
The `polyarray` container is a dynamic array implementation that supports custom element types, comparison, and manipulation operations. It is designed to work with a custom static heap manager and offers flexibility by utilizing function pointers for operations like comparison, swapping, and printing elements.

### Key Features

1. **Dynamic Allocation and Sizing**:  
   The `polyarray` uses dynamic memory allocation to create a flexible array container that can grow as needed. Memory allocation is handled through a custom static heap manager, making it suitable for constrained environments where memory management is crucial.

2. **Function Pointer Customization**:  
   The container supports function pointers for:
   - Element comparison (`cmp`)
   - Less-than operation (`lessthan`)
   - Swapping elements (`swap`)
   - Printing elements (`print`)

   This design allows for seamless integration with different data types and custom operations.

3. **Opaque Interface and Encapsulation**:  
   The container is accessed through an opaque pointer (`polyptr`), abstracting away implementation details from the user and enforcing encapsulation (Pimpl idiom).

4. **Generic Algorithms Support**:  
   The `polyarray` integrates with a set of generic algorithms (defined in `algo.c`) such as:
   - **Sorting**: Sorts the array using the provided comparison function.
   - **Searching**: Performs binary search on sorted arrays.
   - **Custom Algorithms**: The container can be extended to support additional algorithms as needed.

### Major Functions

1. **`poly_alloc`**:
   - Allocates and initializes a `polyarray` with a given capacity and element size.
   - Allows the user to define custom comparison, less-than, swap, and print functions.

   ```c
   polyptr poly_alloc(const size_t capacity, const size_t datasize,
                      int (*cmp)(const genptr, const genptr),
                      bool(*lessthan)(const genptr, const genptr),
                      void(*swap)(genptr, genptr),
                      void(*print)(const genptr));
   ```

2. **`poly_add`**:
   - Adds a range of bytes to the array.
   - Ensures the added data does not exceed the capacity.

   ```c
   void poly_add(polyptr pa, const genptr data, const size_t count);
   ```

3. **`poly_at` and `poly_set`**:
   - `poly_at`: Provides read/write access to a specific element.
   - `poly_set`: Sets the value of an element at a specified position.

   ```c
   iterator poly_at(polyptr pa, const size_t pos);
   void poly_set(polyptr pa, const genptr val, const size_t pos);
   ```

4. **`poly_sort` and `poly_search`**:
   - `poly_sort`: Sorts the array using a generic sorting algorithm.
   - `poly_search`: Searches for an element in a sorted array.

   ```c
   void poly_sort(polyptr pa);
   genptr poly_search(const polyptr pa, const genptr val);
   ```

5. **`poly_free`**:
   - Releases the allocated memory for the array and its elements.

   ```c
   void poly_free(polyptr pa);
   ```

### Usage Example

```c
polyptr int_array = poly_alloc(100, sizeof(int), int_cmp, int_less, int_swap, print_int);
int values[] = {5, 3, 7, 2, 8};
poly_add(int_array, values, 5);
poly_sort(int_array);
poly_print(int_array);
poly_free(int_array);
```

### Summary
The `polyarray` container provides a powerful, flexible, and type-agnostic dynamic array solution that is highly customizable through the use of function pointers and custom memory management. This container is well-suited for use in systems with strict memory constraints and a need for encapsulated, dynamic data structures. Its integration with generic algorithms makes it a versatile choice for a variety of applications.
