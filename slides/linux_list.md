/**
 * @page linux_list Linux Kernel-Style Linked List Implementation
 * @brief Overview of the Linux kernel-style linked list implementation adapted for the MLibs project.
 *
 * ## Linux Kernel-Style Linked List Overview
 *
 * The `linux_list` module provides a simple and efficient doubly linked list implementation based on the Linux kernel's list API. This adaptation retains the core features and design principles of the original Linux kernel implementation, making it suitable for generic use in various C projects.
 *
 * ### Key Features
 *
 * - **Doubly Linked List**: Supports bidirectional traversal through both forward and backward pointers.
 * - **Embedded List Head**: The list is managed through an embedded `struct list_head` within the elements, minimizing memory overhead and enhancing cache locality.
 * - **Minimal Overhead**: The design is lightweight and does not require additional memory allocations beyond the elements themselves.
 * - **Flexible Integration**: Can be used to build complex data structures like queues, stacks, and circular buffers.
 *
 * ### Major Macros and Functions
 *
 * - **`LIST_HEAD_INIT`**:
 *   - Initializes a `list_head` structure, setting both the `next` and `prev` pointers to point to itself, creating an empty list.
 *   - Macro Example:
 *     ```c
 *     LIST_HEAD(my_list);
 *     ```
 *
 * - **`INIT_LIST_HEAD`**:
 *   - Initializes an existing `list_head` structure to create an empty list.
 *   - Macro Example:
 *     ```c
 *     struct list_head my_list;
 *     INIT_LIST_HEAD(&my_list);
 *     ```
 *
 * - **`list_add`**:
 *   - Adds a new element to the list immediately after the specified head.
 *   - Macro Example:
 *     ```c
 *     struct list_head new_element;
 *     list_add(&new_element, &my_list);
 *     ```
 *
 * - **`list_add_tail`**:
 *   - Adds a new element to the list just before the specified head.
 *   - Useful for implementing queues and other FIFO structures.
 *
 * - **`list_del`**:
 *   - Deletes an element from the list by connecting its `prev` and `next` pointers.
 *   - Does not free memory, only removes the element from the list.
 *
 * - **`list_for_each`**:
 *   - Iterates through each element in the list.
 *   - Macro Example:
 *     ```c
 *     struct list_head *pos;
 *     list_for_each(pos, &my_list) {
 *         // Process each element pointed to by `pos`
 *     }
 *     ```
 *
 * ### Additional Helper Functions and Macros
 *
 * - **`list_empty`**:
 *   - Checks whether a given list is empty.
 *   - Returns `true` if the list has no elements, otherwise `false`.
 *
 * - **`list_entry`**:
 *   - Retrieves the pointer to the parent structure from a `list_head` pointer.
 *   - This macro enables integration of the list within complex data structures.
 *
 * - **`list_splice` and `list_splice_init`**:
 *   - Splices two lists together, either combining or moving elements from one list to another.
 *   - The `list_splice_init` variant reinitializes the emptied list after splicing.
 *
 * ### Usage Example
 *
 * ```c
 * struct my_data {
 *     int value;
 *     struct list_head list;  // Embedded list head within the structure
 * };
 *
 * struct my_data item1, item2;
 * LIST_HEAD(my_list);
 * 
 * // Initialize and add items to the list
 * INIT_LIST_HEAD(&item1.list);
 * INIT_LIST_HEAD(&item2.list);
 * list_add(&item1.list, &my_list);
 * list_add_tail(&item2.list, &my_list);
 *
 * // Iterate through the list and print values
 * struct list_head *pos;
 * struct my_data *entry;
 * list_for_each(pos, &my_list) {
 *     entry = list_entry(pos, struct my_data, list);
 *     printf("Value: %d\n", entry->value);
 * }
 * ```
 *
 * ### Summary
 * The `linux_list` module provides a highly efficient and flexible linked list implementation derived from the Linux kernel. It is ideal for use in a variety of applications that require linked list data structures, providing both ease of use and minimal memory overhead. The design is robust, extensible, and well-suited for use in both user-space and embedded environments.
 */

This is a modified version of the Linux Kernel's linked list implementation, structured as a set of macros and inline functions for manipulating doubly linked lists. This library provides a flexible, efficient, and type-agnostic implementation that is suitable for various use cases in C programming. Let's look at some key components and functionality provided:

### Overview of `list.h`

1. **Structure Definition (`struct list_head`)**:  
   The library defines a basic doubly linked list node structure, which consists of two pointers, `next` and `prev`, pointing to the next and previous nodes in the list respectively.

   ```c
   struct list_head {
       struct list_head *next, *prev;
   };
   ```

2. **Macros for List Initialization**:
   - `LIST_HEAD_INIT(name)`: Initializes a list head with pointers pointing to itself.
   - `INIT_LIST_HEAD(ptr)`: Initializes a dynamically allocated list head.

3. **Adding and Removing Nodes**:
   - `list_add`: Adds a new entry after the specified head, ideal for implementing stacks.
   - `list_add_tail`: Adds a new entry before the specified head, suitable for implementing queues.
   - `list_del`: Removes an entry from the list and sets its pointers to `NULL`.
   - `list_del_init`: Deletes an entry and reinitializes it to an empty state.

4. **Moving and Splicing Nodes**:
   - `list_move`: Moves an entry to the head of another list.
   - `list_move_tail`: Moves an entry to the tail of another list.
   - `list_splice` and `list_splice_init`: Joins two lists together and reinitializes the source list if needed.

5. **Traversal and Iteration**:
   - `list_for_each`: Iterates over a list in the forward direction.
   - `list_for_each_prev`: Iterates over a list in the backward direction.
   - `list_for_each_safe`: Safely iterates over a list while allowing for node deletions during iteration.

6. **Type-Specific Iteration**:
   - `list_for_each_entry`: Iterates over list nodes of a specified type.
   - `list_for_each_entry_safe`: Safely iterates over list nodes of a specified type, allowing for deletions during iteration.

### Key Use Cases

- **Implementing Stacks and Queues**:  
  The `list_add` and `list_add_tail` functions make it easy to implement stack-like (LIFO) and queue-like (FIFO) data structures.

- **Traversal and Search**:  
  The `list_for_each` and `list_for_each_entry` macros enable straightforward traversal of lists without requiring explicit pointer manipulation.

- **Memory Management**:  
  The list manipulation functions provide safe memory management by ensuring that nodes are properly linked and unlinked, reducing the risk of memory corruption or leaks.

### Example Usage

```c
#include "list.h"
#include <stdio.h>

struct MyStruct {
    int value;
    struct list_head list;  // List node
};

void print_list(struct list_head *head) {
    struct MyStruct *entry;
    list_for_each_entry(entry, head, list) {
        printf("Value: %d\n", entry->value);
    }
}

int main() {
    LIST_HEAD(my_list);  // Create and initialize an empty list

    struct MyStruct item1 = { .value = 10 };
    struct MyStruct item2 = { .value = 20 };
    struct MyStruct item3 = { .value = 30 };

    // Add items to the list
    list_add(&item1.list, &my_list);
    list_add_tail(&item2.list, &my_list);
    list_add_tail(&item3.list, &my_list);

    // Print list contents
    print_list(&my_list);

    // Remove item2 from the list
    list_del(&item2.list);

    // Print list contents again
    printf("After removing item2:\n");
    print_list(&my_list);

    return 0;
}
```

### Summary

This library provides a simple yet powerful mechanism for managing linked lists in C. It abstracts away much of the pointer arithmetic and error-prone list operations, making it easier to write clean and safe code for managing dynamic data structures. It's widely used in the Linux kernel and other low-level projects, demonstrating its efficiency and robustness.