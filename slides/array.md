/**
 * @page array Array Module
 * @brief Overview of the dynamic array module and its functionality.
 *
 * ## Array Overview
 *
 * The `array` module provides a dynamic array implementation that can be used in embedded environments. It uses a custom static heap manager for memory allocation and provides an interface similar to the C++ `std::vector` but tailored for C.
 *
 * ### Key Features
 * - **Dynamic Allocation and Sizing**: Supports the creation of dynamic arrays with custom capacities.
 * - **Array Manipulation Functions**: Includes functions for accessing, modifying, and sorting array elements.
 * - **Encapsulation and Type Safety**: Uses the Pimpl idiom to enforce encapsulation and provide an opaque interface for arrays.
 * - **Integration with Algorithmic Functions**: Can be used with various algorithms such as sorting, searching, and transformations defined in `algo.c`.
 *
 * ### Major Functions
 * - **`array_alloc`**: Allocates and initializes a dynamic array with a specified capacity and element size.
 * - **`array_add`**: Adds elements to the array.
 * - **`array_at` and `array_set`**: Provides access and modification capabilities for individual elements.
 * - **`array_sort`, `array_search`, `array_modify`**: Algorithms specifically adapted for dynamic arrays.
 *
 * ### Usage Example
 *
 * ```c
 * arrayptr int_array = array_alloc(100, sizeof(int));
 * int values[] = {5, 3, 7, 2, 8};
 * array_add(int_array, values, 5);
 * array_sort(int_array, int_less, int_swap);
 * array_print(int_array, print_int);
 * array_free(int_array);
 * ```
 *
 * See the detailed documentation for each function in the array module for more information.
 */

### Dynarray Container Overview

**Purpose**:  
The `dynarray` container is a dynamic array implementation that provides basic operations such as addition, sorting, searching, and transformation of elements. It is designed to work with the custom static heap manager and offers support for dynamically allocated or externally managed memory.

### Key Features

1. **Dynamic Allocation and Sizing**:  
   The `dynarray` supports dynamic memory allocation, allowing the container to grow as needed. The memory allocation is handled through a custom static heap manager, making it suitable for constrained environments.

2. **Multiple Memory Management Modes**:  
   - **`array_alloc`**: Allocates memory internally, creating a standalone dynamic array that is managed within the library.
   - **`array_attach`**: Attaches to an external memory buffer, allowing the `dynarray` to manage elements within pre-allocated or shared memory. This is useful in scenarios where memory is shared between multiple data structures or devices.

3. **Encapsulation and Opaque Interface**:  
   The container is accessed through an opaque pointer (`arrayptr`), abstracting away implementation details from the user. This makes the interface more robust and easier to maintain.

4. **Integration with Custom Algorithms**:  
   The `dynarray` integrates seamlessly with a set of generic algorithms (defined in `algo.c`), such as sorting, searching, and transforming elements. This makes it a versatile data structure for various operations, similar to `std::vector` in C++.

5. **Flexible Element Handling**:  
   The container uses generic pointers (`genptr`) to handle elements of any data type. Users can define their own comparison, transformation, and modification functions, making the container highly adaptable for different data types and operations.

### Major Functions

1. **`array_alloc` and `array_attach`**:
   - `array_alloc`: Allocates memory for a dynamic array with the specified capacity and element size.
   - `array_attach`: Attaches the container to an external memory buffer, allowing it to manage elements within pre-existing memory.

   ```c
   arrayptr array_alloc(const size_t capacity, const size_t datasize);
   arrayptr array_attach(const genptr base, const size_t capacity, const size_t datasize);
   ```

2. **`array_add`**:
   - Adds a range of bytes to the array, ensuring that the added data does not exceed the container's capacity.

   ```c
   void array_add(arrayptr pa, const genptr data, const size_t count);
   ```

3. **`array_at` and `array_set`**:
   - `array_at`: Provides read/write access to a specific element in the array.
   - `array_set`: Sets the value of an element at a specified position.

   ```c
   iterator array_at(arrayptr pa, const size_t pos);
   void array_set(arrayptr pa, const genptr val, const size_t pos);
   ```

4. **`array_sort`, `array_search`, and `array_modify`**:
   - `array_sort`: Sorts the array using a user-defined comparison and swap function.
   - `array_search`: Searches for an element in a sorted array using a comparison function.
   - `array_modify`: Modifies each element in the array using a provided modification function.

   ```c
   void array_sort(arrayptr pa, bool (*cmp)(const genptr v1, const genptr v2), void (*swap)(genptr, genptr));
   genptr array_search(const arrayptr pa, const genptr val, int (*cmp)(const genptr, const genptr));
   void array_modify(arrayptr pa, void (*mod)(genptr p));
   ```

5. **`array_free` and `array_detach`**:
   - `array_free`: Frees the allocated memory for the array and its elements.
   - `array_detach`: Detaches the container from an external buffer without deallocating the memory.

   ```c
   void array_free(arrayptr pa);
   void array_detach(arrayptr pa);
   ```

6. **`array_transform`**:
   - Transforms elements from a source array to a destination array using a user-defined transformation function.

   ```c
   void array_transform(arrayptr dest, const arrayptr src, void (*func)(const genptr s, genptr d));
   ```

### Usage Example

```c
// Allocating an array for integers
arrayptr int_array = array_alloc(100, sizeof(int));

// Adding values to the array
int values[] = {5, 3, 7, 2, 8};
array_add(int_array, values, 5);

// Sorting the array
array_sort(int_array, int_less, int_swap);

// Printing the sorted array
array_print(int_array, print_int);

// Searching for a value in the sorted array
int key = 7;
int* found = (int*)array_search(int_array, &key, int_cmp);
if (found) {
    printf("Found %d in the array.\n", *found);
}

// Modifying all elements in the array
array_modify(int_array, double_value);  // Assume `double_value` is a user-defined function

// Freeing the array
array_free(int_array);
```

### Summary
The `dynarray` container is a powerful dynamic array implementation that offers a high degree of flexibility through its use of custom memory management and integration with generic algorithms. The ability to work with both internally managed and externally attached memory makes it suitable for a wide range of applications, particularly in systems with stringent memory requirements. Its support for user-defined operations through function pointers enables a broad spectrum of customizations, making it a versatile choice for developers looking for dynamic data structures in C.