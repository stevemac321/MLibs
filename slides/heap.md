/**
 * @page heap Custom Heap Implementation
 * @brief Overview of the heap module, implementing a custom heap for dynamic memory allocation.
 *
 * ## Heap Module Overview
 *
 * The `heap` module provides a custom heap implementation for dynamic memory allocation, using a contiguous array to represent a heap data structure. This module offers a lightweight alternative to standard library functions like `malloc` and `free`, making it ideal for constrained environments, such as embedded systems. The heap is organized as a series of blocks with headers and trailers that record the allocation status and size of each block.
 *
 * ### Key Features
 * - **Custom Memory Allocation**: Implements functions similar to standard `malloc`, `calloc`, `realloc`, and `free`.
 * - **Block-Based Memory Management**: Manages memory using blocks with headers and trailers, which helps in maintaining heap consistency.
 * - **First-Fit Allocation**: Uses a first-fit strategy for finding free blocks.
 * - **Heap Validation and Testing**: Includes functions for validating heap integrity and gathering heap usage statistics.
 *
 * ### Major Functions
 *
 * - **Initialization**:
 *   - `long Heap_Init(void)`: Initializes the heap, setting it to a clean state where no memory is allocated.
 *
 * - **Memory Allocation and Deallocation**:
 *   - `void* Heap_Malloc(long desiredBytes)`: Allocates memory of the given size, similar to `malloc`.
 *   - `void* Heap_Calloc(long desiredBytes)`: Allocates memory and initializes it to zero, similar to `calloc`.
 *   - `void* Heap_Realloc(void* oldBlock, long desiredBytes)`: Reallocates a previously allocated block to a new size, similar to `realloc`.
 *   - `long Heap_Free(void* pointer)`: Frees a previously allocated block of memory, returning it to the heap.
 *
 * - **Heap Testing and Statistics**:
 *   - `long Heap_Test(void)`: Tests the heap for corruption or inconsistencies.
 *   - `heap_stats_t Heap_Stats(void)`: Returns statistics on current heap usage, including the number of allocated and free blocks, and the total heap overhead.
 *
 * ### Helper Functions
 *
 * - **Block Manipulation and Validation**:
 *   - `static long inHeapRange(long* address)`: Checks if an address is within the heap range.
 *   - `static long blockUsed(long* block)`: Determines if a block is marked as used.
 *   - `static long blockUnused(long* block)`: Determines if a block is marked as unused.
 *   - `static long* blockHeader(long* blockEnd)`: Returns the header of a block given its trailer.
 *   - `static long* blockTrailer(long* blockStart)`: Returns the trailer of a block given its header.
 *
 * - **Block Splitting and Merging**:
 *   - `static long splitAndMarkBlockUsed(long* upperBlockStart, long desiredRoom)`: Splits a block into two if the size permits, marking the upper block as used.
 *   - `static void mergeBlockWithBelow(long* upperBlockStart)`: Merges a block with the block below it, if they are both free.
 *
 * ### Structure Definitions
 * - **`struct heap_stats_t`**:
 *   Contains information on the current status of the heap, such as the number of allocated and free blocks, and the total number of words available and allocated.
 *
 * ### Usage Example
 *
 * ```c
 * Heap_Init();  // Initialize the heap
 *
 * // Allocate memory for 100 integers
 * int* data = (int*)Heap_Malloc(100 * sizeof(int));
 * if (data) {
 *     // Use the allocated memory
 *     data[0] = 42;
 *
 *     // Free the allocated memory
 *     Heap_Free(data);
 * }
 * ```
 *
 * ### Summary
 * The `heap` module provides a robust custom heap implementation for dynamic memory allocation in constrained environments. It supports common allocation patterns found in `malloc`, `calloc`, and `realloc`, while providing additional features like heap validation and statistics. This module can be a valuable component for applications requiring custom memory management or a lightweight alternative to standard libraries.
 */

**Purpose**:  
The `heap.c` file implements a custom dynamic memory allocator tailored for embedded systems using a Knuth-style heap structure. This allocator manages memory in a first-fit manner, allocating, reallocating, and freeing memory blocks based on the `malloc`, `calloc`, `realloc`, and `free` APIs commonly used in C programming. This library is particularly useful in environments with limited memory and strict performance requirements, such as real-time operating systems (RTOS).

### Key Features

1. **Custom Static Heap Array**:  
   - The heap is implemented using a static array, `Heap`, which represents the entire available memory pool.
   - Memory blocks are managed with headers and trailers that store metadata about each block's size and status (used or unused).
   - The `Heap_Init()` function sets up the initial state of the heap with a single, large, unused block.

2. **Block Metadata Management**:  
   - Each block in the heap contains a header and trailer, recording the number of words it can store.
   - Positive values in headers and trailers indicate that a block is used, while negative values indicate that a block is free.
   - Functions like `blockUsed`, `blockUnused`, and `blockRoom` are used to manipulate and query the state of each block.

3. **Memory Allocation Functions**:
   - **`Heap_Malloc`**: Allocates a block of memory, returning a pointer to the allocated space. If a block is larger than required, it splits the block and marks the remaining portion as free.
   - **`Heap_Calloc`**: Allocates memory and initializes all bytes in the allocated block to zero.
   - **`Heap_Realloc`**: Changes the size of an allocated block, possibly moving it to a new location if necessary.
   - **`Heap_Free`**: Frees a previously allocated block and merges adjacent free blocks to reduce fragmentation.

4. **Heap Integrity and Statistics**:
   - **`Heap_Test`**: Validates the integrity of the heap, checking for consistency in headers and trailers.
   - **`Heap_Stats`**: Reports on the current usage of the heap, including the number of allocated and free blocks, total words allocated, and overhead.

### Major Functions

1. **`Heap_Init`**:
   - Initializes the heap with a single, large, free block that spans the entire heap array.

   ```c
   long Heap_Init(void);
   ```

2. **`Heap_Malloc`**:
   - Allocates memory from the heap, using a first-fit strategy to find the first available block that is large enough to satisfy the request.
   - If the block is larger than needed, it splits the block and uses only the required portion.

   ```c
   void* Heap_Malloc(long desiredBytes);
   ```

3. **`Heap_Calloc`**:
   - Allocates a block of memory and initializes all bytes in the block to zero.

   ```c
   void* Heap_Calloc(long desiredBytes);
   ```

4. **`Heap_Realloc`**:
   - Resizes a previously allocated block, potentially moving it to a new location if necessary.

   ```c
   void* Heap_Realloc(void* oldBlock, long desiredBytes);
   ```

5. **`Heap_Free`**:
   - Frees a block of memory, merging it with adjacent free blocks if possible.

   ```c
   long Heap_Free(void* pointer);
   ```

6. **`Heap_Test`**:
   - Verifies the integrity of the heap, checking for corrupted headers or trailers, and ensuring there are no consecutive free blocks.

   ```c
   long Heap_Test(void);
   ```

7. **`Heap_Stats`**:
   - Returns a structure containing the current usage statistics of the heap, including the number of used and unused blocks.

   ```c
   heap_stats_t Heap_Stats(void);
   ```

### Implementation Details

1. **Memory Blocks and Metadata**:
   - Each block in the heap has a header and a trailer, storing the size of the block (number of words it can hold).
   - A positive value in the header or trailer indicates that the block is in use.
   - A negative value indicates that the block is free.

2. **Block Splitting and Merging**:
   - When a block is allocated, if it is larger than necessary, it can be split into two blocks: the first block satisfies the allocation, and the second block remains free.
   - When a block is freed, it can be merged with adjacent free blocks to form a larger contiguous block, reducing fragmentation.

3. **First-Fit Allocation Strategy**:
   - The `Heap_Malloc` function traverses the heap looking for the first free block that is large enough to satisfy the allocation request.
   - This approach is simple and efficient but can lead to fragmentation over time.

### Usage Example

```c
#include "heap.h"

int main() {
    Heap_Init();  // Initialize the heap

    // Allocate 100 bytes
    void* ptr1 = Heap_Malloc(100);
    
    // Allocate another 200 bytes
    void* ptr2 = Heap_Malloc(200);

    // Free the first block
    Heap_Free(ptr1);

    // Check heap status
    heap_stats_t stats = Heap_Stats();

    return 0;
}
```

### Summary

The `heap.c` library provides a robust memory management solution for embedded systems that require dynamic memory allocation. Its use of a fixed-size array and detailed block metadata makes it suitable for real-time systems with constrained memory resources. By leveraging splitting and merging of blocks, it minimizes fragmentation and maintains the integrity of the heap, ensuring predictable memory allocation patterns. The library's integration with various allocation and reallocation functions makes it a flexible choice for memory management in embedded applications.