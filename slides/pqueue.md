/**
 * @page pqueue Priority Queue Implementation
 * @brief Overview of the Priority Queue implementation for the MLibs project.
 *
 * ## Priority Queue Overview
 *
 * The `priority_queue` is a heap-based implementation that allows for efficient insertion, deletion, and retrieval of elements according to their priority. It is implemented using an array-based representation of a binary heap, making it suitable for systems with constrained resources.
 *
 * ### Key Features
 *
 * - **Array-Based Heap Implementation**:
 *   - The priority queue uses an array-based binary heap to organize elements.
 *   - Elements can be efficiently added, removed, and sorted based on their priority values.
 *
 * - **Heap Property Maintenance**:
 *   - The queue ensures that the heap property is maintained during operations, making it a max-heap by default.
 *   - The implementation leverages helper functions such as `max_heapify`, `priority_queue_build_max_heap`, and `priority_queue_add` to manage heap structure.
 *
 * - **Dynamic Adjustments**:
 *   - Supports dynamic insertion and deletion of elements.
 *   - Can adjust to changes in priority values with efficient reordering of elements.
 *
 * - **Support for Custom Operations**:
 *   - Function pointers and algorithms can be used to customize the comparison, printing, and manipulation of heap elements.
 *
 * ### Major Functions
 *
 * - **`priority_queue_from_array`**:
 *   - Initializes the priority queue from an existing array of values.
 *   - Function Signature:
 *     ```c
 *     void priority_queue_from_array(struct priority_queue *pq, float input[], const size_t size);
 *     ```
 *
 * - **`priority_queue_build_max_heap`**:
 *   - Builds a max-heap from the existing array in the priority queue.
 *   - Function Signature:
 *     ```c
 *     void priority_queue_build_max_heap(struct priority_queue *pq);
 *     ```
 *
 * - **`priority_queue_add`**:
 *   - Inserts a new element into the heap and maintains the heap property.
 *   - Function Signature:
 *     ```c
 *     void priority_queue_add(struct priority_queue *pq, float value);
 *     ```
 *
 * - **`priority_queue_pop`**:
 *   - Removes and returns the highest priority element from the heap.
 *   - Function Signature:
 *     ```c
 *     bool priority_queue_pop(struct priority_queue *pq, float *value);
 *     ```
 *
 * - **`priority_queue_is_empty`**:
 *   - Checks if the heap is empty.
 *   - Function Signature:
 *     ```c
 *     bool priority_queue_is_empty(const struct priority_queue *pq);
 *     ```
 *
 * - **`priority_queue_print_heap`**:
 *   - Prints the contents of the heap for debugging purposes.
 *   - Function Signature:
 *     ```c
 *     void priority_queue_print_heap(const struct priority_queue *pq);
 *     ```
 *
 * ### Internal Helper Functions
 *
 * - **`parent`**:
 *   - Returns the index of the parent of the element at index `i`.
 *   - Function Signature:
 *     ```c
 *     static size_t parent(size_t i);
 *     ```
 *
 * - **`left`**:
 *   - Returns the index of the left child of the element at index `i`.
 *   - Function Signature:
 *     ```c
 *     static size_t left(size_t i);
 *     ```
 *
 * - **`right`**:
 *   - Returns the index of the right child of the element at index `i`.
 *   - Function Signature:
 *     ```c
 *     static size_t right(size_t i);
 *     ```
 *
 * - **`max_heapify`**:
 *   - Maintains the max-heap property starting from a given index `i`.
 *   - Function Signature:
 *     ```c
 *     static void max_heapify(struct priority_queue *pq, size_t i);
 *     ```
 *
 * - **`swap`**:
 *   - Swaps two elements in the heap.
 *   - Function Signature:
 *     ```c
 *     static void swap(float *a, float *b);
 *     ```
 *
 * ### Usage Example
 *
 * ```c
 * // Initialize a priority queue from an array of float values.
 * float values[] = {5.3, 7.2, 1.8, 4.9, 2.6};
 * struct priority_queue pq;
 * priority_queue_from_array(&pq, values, sizeof(values) / sizeof(float));
 *
 * // Build the max-heap and print the heap.
 * priority_queue_build_max_heap(&pq);
 * priority_queue_print_heap(&pq);
 *
 * // Insert a new value and print the heap again.
 * priority_queue_add(&pq, 8.4);
 * priority_queue_print_heap(&pq);
 *
 * // Pop the highest priority element.
 * float highest;
 * if (priority_queue_pop(&pq, &highest)) {
 *     printf("Popped value: %.2f\n", highest);
 * }
 * ```
 *
 * ### Summary
 *
 * The `priority_queue` is a versatile and efficient data structure that supports custom operations and dynamic changes in priority values. Its array-based heap implementation ensures predictable memory usage and efficient operation. It is ideal for applications requiring priority-based data organization and retrieval.
 */

### Priority Queue Overview

**Purpose**:  
The `priority_queue` implementation uses a max-heap data structure to manage a collection of floating-point numbers in a way that the maximum element is always at the root of the heap. This design allows efficient insertion, deletion, and access to the maximum element, making it suitable for scenarios like task scheduling, simulations, or priority-based problem-solving.

### Key Features

1. **Max-Heap Property**:
   - The priority queue ensures that the maximum value is always at the root of the heap.
   - The `max_heapify` function maintains the max-heap property by recursively adjusting elements.

2. **Dynamic Array Integration**:
   - The `priority_queue` can be built directly from an array of float values using the `priority_queue_from_array` function, making it versatile and easy to integrate with existing data.

3. **Insertion and Removal**:
   - `priority_queue_add` inserts a new element into the heap while maintaining the max-heap property.
   - `priority_queue_pop` removes and returns the maximum element, readjusting the heap to ensure it remains valid.

4. **Heap Visualization**:
   - The `priority_queue_print_heap` function provides a debug-friendly way to print the heap contents, which helps visualize the internal state of the heap for debugging and validation.

### Major Functions

1. **`priority_queue_from_array`**:
   - Initializes a priority queue with values from an existing array, allowing the heap to be built directly from a pre-populated dataset.

   ```c
   void priority_queue_from_array(struct priority_queue *pq, float input[], const size_t size) {
       pq->length = size;
       pq->heap_size = size;
       pq->heap = input;
   }
   ```

2. **`priority_queue_build_max_heap`**:
   - Converts an unordered array into a valid max-heap.
   - This function is useful for transforming an existing array of elements into a heap, enabling quick access to the maximum element.

   ```c
   void priority_queue_build_max_heap(struct priority_queue *pq) {
       for (size_t i = pq->heap_size / 2; i > 0; i--) {
           max_heapify(pq, i - 1);
       }
   }
   ```

3. **`priority_queue_add`**:
   - Adds a new element to the heap while maintaining the max-heap property.
   - If the heap is at capacity, the function will exit without adding the element.

   ```c
   void priority_queue_add(struct priority_queue *pq, float value) {
       if (pq->heap_size == pq->length) {
           return;  // Heap is full
       }
       pq->heap[pq->heap_size] = value;
       pq->heap_size++;
       priority_queue_build_max_heap(pq);
   }
   ```

4. **`priority_queue_pop`**:
   - Removes and returns the maximum element from the heap.
   - The heap is restructured to maintain the max-heap property after removal.

   ```c
   bool priority_queue_pop(struct priority_queue *pq, float *value) {
       if (pq->heap_size == 0) {
           return false;  // No elements left to pop
       }

       *value = pq->heap[0];
       pq->heap[0] = pq->heap[pq->heap_size - 1];
       pq->heap_size--;
       max_heapify(pq, 0);

       return true;
   }
   ```

5. **`priority_queue_is_empty`**:
   - Checks if the priority queue is empty by evaluating the heap size.

   ```c
   bool priority_queue_is_empty(const struct priority_queue *pq) {
       return pq->heap_size == 0;
   }
   ```

6. **`priority_queue_print_heap`**:
   - Prints the contents of the heap along with their indices for easy visualization and debugging.

   ```c
   void priority_queue_print_heap(const struct priority_queue *pq) {
       printf("\nPriority Queue Dump: \r\n");
       for (size_t i = 0; i < pq->heap_size; i++) {
           printf("Index %d, Data: %.6f\r\n", i, pq->heap[i]);
       }
   }
   ```

### Example Usage

```c
#include "pqueue.h"

int main() {
    struct priority_queue pq;
    float data[] = {3.2f, 4.5f, 1.1f, 5.9f, 2.3f};
    priority_queue_from_array(&pq, data, 5);

    // Build the max heap
    priority_queue_build_max_heap(&pq);

    // Print the initial state of the heap
    priority_queue_print_heap(&pq);

    // Pop and print the top element
    float value;
    if (priority_queue_pop(&pq, &value)) {
        printf("Popped value: %.6f\n", value);
    }

    return 0;
}
```

### Summary

The `priority_queue` implementation offers a straightforward yet powerful way to manage collections of elements using a max-heap. Its flexibility to build a heap from an existing array, combined with efficient insertion and removal operations, makes it ideal for applications that require frequent access to the maximum element. Additionally, the inclusion of debug functions like `priority_queue_print_heap` ensures ease of use and visibility into the heap’s internal structure, facilitating both development and testing.