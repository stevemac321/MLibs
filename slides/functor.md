/**
 * @page functor Functors and Generic Operations
 * @brief Overview of the functor and generic operations provided in `functor.c`.
 *
 * ## Functor Utilities Overview
 *
 * The `functor` module provides a set of utilities for defining and manipulating custom operations on various data types. These utilities include functions for comparison, printing, swapping, and transforming elements. The functor functions are designed to be compatible with the `algo.c` module, enabling powerful generic algorithms for different data structures.
 *
 * ### Key Features
 * - **Custom Comparison Functions**: Define comparison criteria for different data types, such as integers, strings, and user-defined structures (e.g., `student`).
 * - **Element Swapping**: Provide element swapping functions that can be used with sorting and other rearrangement algorithms.
 * - **Printing Utilities**: Print elements in different formats, including integers, hexadecimal, and strings, for easy debugging and visualization.
 * - **Generic Transformations**: Modify and transform elements within a container using generic function pointers.
 *
 * ### Major Functions
 * - **Comparison Functions**:
 *   - `bool int_less(const int *v1, const int *v2)`: Returns true if `*v1` is less than `*v2`.
 *   - `int int_cmp(const int *v1, const int *v2)`: Compares two integers and returns a value similar to `strcmp`.
 *   - `bool str_less(const char **s1, const char **s2)`: Compares two strings lexicographically.
 *   - `int str_cmp(const char **s1, const char **s2)`: String comparison function using `strcmp` semantics.
 *   - `bool record_gpaless(const student *v1, const student *v2)`: Compares two `student` structures based on GPA and name.
 *
 * - **Swap Functions**:
 *   - `void int_swap(int *v1, int *v2)`: Swaps two integer values.
 *   - `void str_swap(char **v1, char **v2)`: Swaps two string pointers.
 *   - `void record_swap(student *v1, student *v2)`: Swaps two `student` structures.
 *
 * - **Printing Functions**:
 *   - `void print_int(const int *el)`: Prints an integer.
 *   - `void print_hex(const int *el)`: Prints an integer in hexadecimal format.
 *   - `void print_str(const char *s)`: Prints a string.
 *   - `void print_pstr(const char **pp)`: Prints a pointer to a string.
 *   - `void print_student(const student *s)`: Prints the details of a `student` structure.
 *
 * - **Transformation Functions**:
 *   - `void double_int(const int *psrc, int *pdest)`: Doubles the value of the source integer and stores it in the destination.
 *   - `void rand_int(int *p)`: Assigns a random integer to the given pointer.
 *   - `void int_genrand(genptr pi)`: Generates a random integer and stores it at the generic pointer location.
 *
 * ### Usage Example
 *
 * ```c
 * int a = 5, b = 10;
 * int_swap(&a, &b);  // Swaps the values of `a` and `b`
 *
 * const char *s1 = "apple";
 * const char *s2 = "orange";
 * if (str_less(&s1, &s2)) {
 *     printf("s1 is less than s2\n");
 * }
 *
 * student s1 = {"John Doe", 3.5};
 * student s2 = {"Jane Doe", 3.9};
 * bool isLess = record_gpaless(&s1, &s2);
 * ```
 *
 * The `functor` module allows for an easy way to work with user-defined types in the generic algorithms provided by other modules in the library. By defining custom comparison, swap, and print functions, developers can create powerful data structures and algorithms suited to their specific needs.
 */


**Purpose**:  
The `functor.c` file provides a set of reusable function objects, or *functors*, that are designed to work seamlessly with the generic containers and algorithms defined in the library. These functors include comparison operations, element transformations, printing utilities, and element swapping operations. This flexibility allows for a variety of data types to be processed using the same generic algorithms, enhancing code reuse and reducing duplication.

### Key Features

1. **Comparison Functors**:
   - Comparison functors are used to define sorting and searching criteria for different data types, both in their basic and generic forms.
   - Examples:
     - `int_cmp`: Compares two integers.
     - `str_cmp`: Compares two strings.
     - `int_genless`: Generic integer comparison for sorting (`true` if less).
     - `str_genless`: Generic string comparison for sorting (`true` if less).
     - `record_gpaless`: Compares two `student` structures by GPA.

2. **Swapping Functors**:
   - Swapping functors are responsible for interchanging elements during sorting operations.
   - Examples:
     - `int_swap`: Swaps two integer values.
     - `str_swap`: Swaps two string pointers.
     - `record_swap`: Swaps two `student` structures.
     - `str_genswap`: Generic swap for strings.

3. **Transformation Functors**:
   - Transformation functors modify elements, either through arithmetic operations or value generation.
   - Examples:
     - `double_int`: Multiplies an integer by 2.
     - `rand_int`: Assigns a random integer value.
     - `int_genrand`: Generates a random integer for generic types.

4. **Printing Functors**:
   - Printing functors provide formatted output for various data types, making it easier to visualize the contents of data structures.
   - Examples:
     - `print_int`: Prints an integer.
     - `print_hex`: Prints an integer in hexadecimal format.
     - `print_student`: Prints a `student` structure.
     - `print_str`: Prints a string.
     - `print_genpstr`: Prints a generic pointer to a string.

5. **Utility and Predicate Functors**:
   - These functors evaluate conditions and perform utility operations on elements.
   - Examples:
     - `is_even`: Checks if an integer is even.
     - `is_geneven`: Checks if a generic integer is even.
     - `ret_int`: Returns an integer value.
     - `ret_genint`: Returns a generic integer value.

### Detailed Analysis of Selected Functors

1. **Comparison Functors**:
   - `int_cmp` and `str_cmp` are basic comparison functors that return values similar to `strcmp` semantics:
     - `0`: Elements are equal.
     - `< 0`: The first element is less than the second.
     - `> 0`: The first element is greater than the second.

   ```c
   int int_cmp(const int* v1, const int* v2)
   {
       assert(v1 && v2);
       return *v1 - *v2;
   }

   int str_cmp(const char** v1, const char** v2)
   {
       assert(v1 && v2 && *v1 && *v2);
       return strcmp(*v1, *v2);
   }
   ```

2. **Swapping Functors**:
   - `int_swap` and `str_swap` swap the values of their respective data types. These are crucial in sorting and transformation operations.

   ```c
   void int_swap(int *v1, int *v2)
   {
       assert(v1 && v2);
       int tmp = *v1;
       *v1 = *v2;
       *v2 = tmp;
   }

   void str_swap(char **v1, char **v2)
   {
       assert(v1 && *v1 && v2 && *v2);
       char *tmp = *v1;
       *v1 = *v2;
       *v2 = tmp;
   }
   ```

3. **Printing Functors**:
   - These functors print elements using `printf` and are used in visualizing array or container contents.
   - Examples include:
     - `print_int`: Prints an integer.
     - `print_student`: Prints a `student` structure by its `name` and `gpa` fields.

   ```c
   void print_int(const int *el) 
   {
       assert(el); 
       printf("%d ", *el); 
   }

   void print_student(const student *s)
   {
       assert(s);
       printf("%s %.2f  ", s->name, s->gpa);
   }
   ```

4. **Transformation Functors**:
   - `double_int` and `rand_int` provide basic transformations like doubling an integer value or assigning a random integer.

   ```c
   void double_int(const int *psrc, int *pdest)
   {
       assert(psrc && pdest);
       int val = *psrc;
       *pdest = (val * 2);
   }

   void rand_int(int *p) 
   { 
       assert(p);
       *p = rand() % 100; 
   }
   ```

5. **Generic Functors**:
   - Generic functors like `int_genless` and `str_genswap` enable operations on generic data pointers, allowing these functions to work seamlessly with a variety of container types.

   ```c
   bool int_genless(const genptr v1, const genptr v2)
   {
       return *(int*)v1 < *(int*)v2;
   }

   void str_genswap(genptr v1, genptr v2)
   {
       assert(v1 && v2);
       char** pp1 = (char**)v1;
       char** pp2 = (char**)v2;
       char *tmp = *pp1;
       *pp1 = *pp2;
       *pp2 = tmp;
   }
   ```

### Summary

The `functor.c` file enhances the versatility and flexibility of the generic containers and algorithms provided in the library. By implementing a wide array of functors that operate on different data types and generic pointers, this module allows seamless integration with various containers like `dynarray`, `polyarray`, and `heap`. The functors adhere to consistent conventions, making them easy to use and integrate across different algorithms. Overall, these functors contribute significantly to the library's ability to handle diverse data types and operations in a consistent and reusable manner.