/**
 * @page table Open Address Hash Table Implementation
 * @brief Overview of the Open Address Hash Table with Value Semantics for the MLibs project.
 *
 * ## Open Address Hash Table Overview
 *
 * The `open_table` is an implementation of an open-addressing hash table that uses double hashing for collision resolution. It stores elements directly in the table array, offering value semantics. The table is designed for use in embedded systems and constrained environments, utilizing a static heap manager and a bitvector for efficient space utilization.
 *
 * ### Key Features
 *
 * - **Double Hashing for Collision Resolution**:
 *   - The hash table uses two separate hash functions (`h1` and `h2`) to resolve collisions. If a collision occurs at a given index, the second hash function is used to probe for an alternative index.
 *
 * - **Bitvector-Based Slot Management**:
 *   - A `bitvector` is used to track which slots in the hash table are occupied, enabling efficient operations and cluster analysis.
 *
 * - **Value Semantics**:
 *   - The table stores values directly within its slots, as opposed to pointers, making it more suitable for small, fixed-size types like integers or floating-point values.
 *
 * - **Support for Floating-Point Types**:
 *   - The implementation supports `float` values by using a `union` to perform bitwise manipulations on the floating-point values for hashing and comparison.
 *
 * - **Cluster Analysis and Statistics**:
 *   - The hash table can generate reports on the distribution of elements, clusters, and collisions, providing insight into the performance and behavior of the hash function and collision resolution strategy.
 *
 * ### Major Functions
 *
 * - **`open_table_init`**:
 *   - Initializes the hash table with a buffer and hash functions.
 *   - Function Signature:
 *     ```c
 *     void open_table_init(struct open_table *table, TYPE *buffer, const size_t size,
 *                          size_t (*h1)(TYPE), size_t (*h2)(TYPE));
 *     ```
 *
 * - **`open_table_insert`**:
 *   - Inserts a value into the table using quadratic probing for collision resolution.
 *   - Function Signature:
 *     ```c
 *     enum insert_state open_table_insert(struct open_table *table, TYPE value);
 *     ```
 *
 * - **`open_table_cluster_report`**:
 *   - Reports on the clusters formed in the hash table, showing which slots are occupied.
 *   - Function Signature:
 *     ```c
 *     void open_table_cluster_report(const struct open_table *table);
 *     ```
 *
 * - **`analyze_clusters`**:
 *   - Analyzes the clusters and provides detailed statistics on the distribution, including the number of clusters, total cluster length, and the size of the largest cluster.
 *   - Function Signature:
 *     ```c
 *     void analyze_clusters(const struct open_table *table);
 *     ```
 *
 * - **`secondary_hash`**:
 *   - Calculates the secondary hash function for the given value.
 *   - Function Signature:
 *     ```c
 *     size_t secondary_hash(TYPE value);
 *     ```
 *
 * ### Cluster Analysis and Statistics
 *
 * After running the `open_table_insert` function for a series of random values, the hash table produces the following statistics:
 *
 * - **Occupied Buckets**: 514
 * - **Total Collisions**: 219
 * - **Total Duplicates**: 0
 * - **Cluster Count**: 205
 * - **Total Cluster Length**: 514
 * - **Largest Cluster Size**: 11
 * - **Average Cluster Length**: 2.51
 *
 * ### Internal Helper Functions
 *
 * - **`is_slot_occupied`**:
 *   - Checks if a specific slot is occupied using the bitvector.
 *   - Function Signature:
 *     ```c
 *     int is_slot_occupied(uint32_t *bitvector, size_t pos);
 *     ```
 *
 * - **`set_slot_occupied`**:
 *   - Marks a slot as occupied in the bitvector.
 *   - Function Signature:
 *     ```c
 *     void set_slot_occupied(uint32_t *bitvector, size_t pos);
 *     ```
 *
 * ### Usage Example
 *
 * ```c
 * // Initialize the hash table with a buffer and hash functions.
 * float buffer[1028];
 * struct open_table table;
 * open_table_init(&table, buffer, sizeof(buffer) / sizeof(float), inthash, secondary_hash);
 *
 * // Insert random floating-point values into the table.
 * for (size_t i = 0; i < 500; i++) {
 *     float value = randf(-1.0e+38f, 1.0e+38f);
 *     open_table_insert(&table, value);
 * }
 *
 * // Print the cluster report and analyze the distribution.
 * open_table_cluster_report(&table);
 * analyze_clusters(&table);
 * ```
 *
 * ### Summary
 *
 * The `open_table` hash table provides a robust implementation for value semantics in constrained environments. Its use of double hashing, bitvector slot management, and detailed cluster analysis makes it an ideal choice for embedded systems and applications requiring efficient hash-based lookups and storage.
 */

### Open Address Hashtable Overview

**Purpose**:  
The `open_table` container is an open-addressing hashtable that implements value semantics, meaning it stores values directly in the table without relying on pointers. This implementation is particularly well-suited for embedded environments, as it uses a fixed, attachable array for storage and employs a compact bitvector to manage the occupancy state of slots.

### Key Features

1. **Open Addressing with Double Hashing**:  
   The `open_table` uses double hashing for collision resolution, which is a method where two hash functions are combined to spread keys more uniformly across the table. This reduces clustering and minimizes probing sequences when collisions occur.

2. **Compact Occupancy Management Using Bitvector**:  
   A bitvector is used to track whether each slot is empty or occupied, minimizing the memory overhead and providing a simple mechanism to manage occupancy without modifying the stored values.

3. **Value Semantics**:  
   The table directly stores values of a specified type (e.g., `float`), making it ideal for numerical data. This value semantic approach helps avoid pointer overhead, making the table efficient in terms of both time and space.

4. **Fixed Array Integration**:  
   The table can be initialized to use a statically allocated array, ensuring predictable memory usage and suitability for real-time or embedded applications where dynamic memory allocation may be restricted or undesirable.

5. **Clustering Analysis**:  
   The `analyze_clusters` function provides insights into the clustering behavior within the table, including metrics like the number of clusters, total cluster length, and average cluster size. This helps evaluate the efficiency of the hashing scheme and collision resolution.

### Major Functions

1. **`open_table_init`**:
   - Initializes the hashtable with a fixed array and bitvector for occupancy tracking.
   - The function accepts two hash functions (`h1` and `h2`) for double hashing.
   - The array and bitvector are zeroed out to indicate all slots are empty.

   ```c
   void open_table_init(struct open_table *table, TYPE *buffer, const size_t size,
                        size_t (*h1)(TYPE ), size_t (*h2)(TYPE ));
   ```

2. **`open_table_insert`**:
   - Inserts a value into the table using double hashing to resolve collisions.
   - Utilizes quadratic probing to handle collisions during insertion.
   - The function returns a status indicating whether the value was inserted successfully, already exists in the table, or if the table is full.

   ```c
   enum insert_state open_table_insert(struct open_table *table, TYPE value);
   ```

3. **`quadratic_probe`**:
   - Implements quadratic probing to calculate the next slot index in case of a collision.
   - The formula used is: `index = (hash + i^2) % table_size`, where `i` is the probe attempt.

   ```c
   size_t quadratic_probe(size_t hash, int i, size_t table_size);
   ```

4. **`inthash` and `secondary_hash`**:
   - Two separate hash functions used for double hashing.
   - `inthash` performs bit manipulation to spread out bits uniformly, while `secondary_hash` provides a secondary hash value to avoid clustering.

   ```c
   size_t inthash(TYPE value);
   size_t secondary_hash(TYPE value);
   ```

5. **Occupancy Management**:
   - `is_slot_occupied` and `set_slot_occupied` are helper functions that use the bitvector to check and set the occupancy state of a slot.

   ```c
   int is_slot_occupied(uint32_t *bitvector, size_t pos);
   void set_slot_occupied(uint32_t *bitvector, size_t pos);
   ```

6. **Clustering Analysis**:
   - `open_table_cluster_report` prints a visual representation of the table's occupancy state, while `analyze_clusters` provides detailed clustering metrics.

   ```c
   void open_table_cluster_report(const struct open_table *table);
   void analyze_clusters(const struct open_table *table);
   ```

### Example Usage

```c
#include "open_table.h"

int main() {
    // Initialize table with a buffer of floats
    struct open_table table;
    TYPE buffer[BUFFER_SIZE];
    open_table_init(&table, buffer, BUFFER_SIZE, inthash, secondary_hash);

    // Insert random values into the table
    for(size_t i = 0; i < BUFFER_SIZE / 2; i++) {
        float f = randf(MIN_FLOAT, MAX_FLOAT);
        open_table_insert(&table, f);
    }

    // Print and analyze table clusters
    open_table_cluster_report(&table);
    analyze_clusters(&table);

    return 0;
}
```

### Summary

The `open_table` hashtable implementation provides a robust and efficient solution for managing numerical data using open addressing and value semantics. The use of double hashing ensures even distribution of values across the table, reducing the risk of clustering and long probing sequences. With additional features like a compact bitvector for occupancy management and detailed clustering analysis, the `open_table` is well-suited for memory-constrained environments such as embedded systems or applications requiring deterministic performance.