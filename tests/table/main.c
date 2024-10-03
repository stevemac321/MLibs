#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>
#include "harness.h"

#define BUFFER_SIZE 1028
#define TYPE float
TYPE buffer[BUFFER_SIZE];

#define MIN_FLOAT -1.0e+38f  // Minimum float value
#define MAX_FLOAT  1.0e+38f  // Maximum float value

// Counters for analysis
size_t collision_count = 0;
size_t duplicate_count = 0;

struct open_table table;

#define TYPE float  // Define TYPE as float, but can be changed to any type

enum insert_state { INSERTED, TABLEFULL, EXISTS };


/**=============================================================================
 Interface:   open_table

 Purpose:    open_table is an open addressing hash table that uses the static
             heap manager.  It uses double hashing for collision resolution.
==============================================================================*/
struct open_table {
    size_t count;           // Current number of elements in the table.
    size_t table_size;      // Total size of the table.

    size_t (*h1)( TYPE value);  // First hash function.
    size_t (*h2)( TYPE value);  // Second hash function.

    TYPE *table;            // Table array storing TYPE values directly.
    uint32_t *bitvector;    // Bitvector array for empty/full bucket checking.
};
void open_table_init(struct open_table *table, TYPE *buffer, const size_t size,
			size_t (*h1)(TYPE ),	size_t (*h2)(TYPE ));

enum insert_state open_table_insert(struct open_table *table, TYPE value);
size_t quadratic_probe(size_t hash, int i, size_t table_size);
size_t inthash(TYPE value);
size_t secondary_hash(TYPE value);
void open_table_cluster_report(const struct open_table *table);
void analyze_clusters(const struct open_table *table);

int is_slot_occupied(uint32_t *bitvector, size_t pos);
void set_slot_occupied(uint32_t *bitvector, size_t pos);

void dummy();
float randf(float min, float max);

int main()
{

    srand((unsigned int)time(NULL));
    open_table_init(&table, buffer, BUFFER_SIZE, inthash, secondary_hash);
    
    TC_BEGIN("open table test");
	for(size_t i=0; i < BUFFER_SIZE / 2; i++) {
        float f = randf(MIN_FLOAT, MAX_FLOAT);
        open_table_insert(&table, f);
    }
    open_table_cluster_report(&table);
    analyze_clusters(&table);
    REPORT("open table end");
    dummy();

}
void dummy()
{
	printf("foo");
}
float randf(float min, float max) {
    return ((float)rand() / (float)RAND_MAX) * (max - min) + min;
}
size_t secondary_hash(TYPE value) {
    union {
        float f;
        uint32_t i;
    } u;
    u.f = value;

    return (u.i * 0x5bd1e995) ^ (u.i >> 15);  // A different hash calculation
}

void open_table_init(struct open_table *table, TYPE *buffer, const size_t size, 
                     size_t (*h1)(TYPE ), size_t (*h2)(TYPE )) {
    assert(table);
    assert(size % sizeof(uint32_t) == 0);  // Ensure the table size is divisible by 32.

    table->count = 0;
    table->table_size = size;
    table->h1 = h1;
    table->h2 = h2;

    // Allocate memory for table and bitvector. Ensure table has `table_size` slots.
    table->table = buffer;
    table->bitvector = (uint32_t *)malloc((size / sizeof(uint32_t)) * sizeof(uint32_t));

    // Zero out the table and bitvector to indicate all slots are empty.
    memset(table->table, 0, size * sizeof(TYPE));
    memset(table->bitvector, 0, (size / sizeof(uint32_t)) * sizeof(uint32_t));
}

// Helper function to check if a slot is occupied using the bitvector.
int is_slot_occupied(uint32_t *bitvector, size_t pos) {
    size_t index = pos / 32;  // Determine which `uint32_t` element in the bitvector.
    size_t bit = pos % 32;    // Determine which bit within that `uint32_t`.
    int occupied = (bitvector[index] & (1 << bit)) != 0;
    //printf("Bitvector Check: Index = %d, Bit = %d, Occupied = %d\n", index, bit, occupied);
    return occupied;
}

// Helper function to set a slot as occupied in the bitvector.
void set_slot_occupied(uint32_t *bitvector, size_t pos) {
    size_t index = pos / 32;
    size_t bit = pos % 32;
   // printf("Setting Bitvector: Index = %d, Bit = %d\n", index, bit);
    bitvector[index] |= (1 << bit);  // Set the bit at `pos` to 1.
}


// Modified `open_table_insert` function to count collisions and duplicates.
enum insert_state open_table_insert(struct open_table *table, const TYPE value) {
    assert(table);

    // Print the value being inserted for tracking.
    //printf("Inserting value: %f\n", value);

    // Iterate through the table using quadratic probing.
    for (int i = 0; i < table->table_size; i++) {
        size_t primary_hash = table->h1(value);  // Primary hash function.
        size_t idx = quadratic_probe(primary_hash, i, table->table_size);

        //printf("Probing: i = %d, Index = %d, Value = %f\n", i, idx, value);

        // Check if the slot is occupied using the bitvector.
        if (is_slot_occupied(table->bitvector, idx)) {
            collision_count++;  // Increment collision count
            //printf("Collision at index: %d for value: %f\n", idx, value);

            // If the slot is occupied, check if the value at this slot matches the new value.
            if (table->table[idx] == value) {
                duplicate_count++;  // Increment duplicate count
                return EXISTS;      // The value already exists in the table.
            }
        } else {
            // If the slot is not occupied, insert the new value.
            table->table[idx] = value;               // Insert the value directly in the table.
            set_slot_occupied(table->bitvector, idx);  // Set the bit in the bitvector to indicate occupancy.
            table->count++;                          // Increase the element count.
            //printf("Inserted value %f at index %d\n", value, idx);
            return INSERTED;
        }
    }

    // Table is full, cannot insert the value.
    return TABLEFULL;
}

size_t quadratic_probe(size_t hash, int i, size_t table_size) {
    return (hash + i * i) % table_size;
}

size_t inthash(TYPE value) {
    assert(value != 0.0f);  // Ensure value is not zero

    // Convert the float to an unsigned integer representation using bit manipulation.
    union {
        float f;
        uint32_t i;
    } u;

    u.f = value;

    // XOR shift and multiply to spread out the bits more uniformly.
    size_t hash = u.i;
    hash ^= hash >> 17;
    hash *= 0xed5ad4bb;
    hash ^= hash >> 11;
    hash *= 0xac4c1b51;
    hash ^= hash >> 15;

    return hash;
}


// Refactored `open_table_cluster_report` function using the bitvector.
void open_table_cluster_report(const struct open_table *table) {

    // Iterate over the table and check occupancy using the bitvector.
    for (size_t i = 0; i < table->table_size; i++) {
        if (is_slot_occupied(table->bitvector, i)) {
            printf("%s ", "F");  // "F" indicates a filled slot
        } else {
            printf("%s ", "_");  // "_" indicates an empty slot
        }
    }
}

// Refactored `analyze_clusters` function using the bitvector.
void analyze_clusters(const struct open_table *table) {
    enum { IN, OUT } state = OUT;
    size_t cluster_count = 0;
    size_t total_cluster_length = 0;
    size_t largest_cluster = 0;
    size_t current_cluster_length = 0;


    // Iterate over the table and check occupancy using the bitvector.
    for (size_t i = 0; i < table->table_size; i++) {
        if (is_slot_occupied(table->bitvector, i)) {  // Filled slot
            if (state == OUT) {
                state = IN;
                cluster_count++;
                current_cluster_length = 1;
            } else {
                current_cluster_length++;
            }
            total_cluster_length++;
        } else {  // Empty slot
            if (state == IN) {
                state = OUT;
                if (current_cluster_length > largest_cluster) {
                    largest_cluster = current_cluster_length;
                }
                current_cluster_length = 0;  // Reset for next cluster
            }
        }
    }

    // Check if the last cluster is the largest.
    if (state == IN && current_cluster_length > largest_cluster) {
        largest_cluster = current_cluster_length;
    }
    printf("Table size: %d\n", table->table_size);
    printf("Occurpied buckets: %d\n", table->count);
    printf("Total Collisions: %d\n", collision_count);
    printf("Total Duplicates: %d\n", duplicate_count);
    printf("Cluster Count: %d\n", cluster_count);
    printf("Total Cluster Length: %d\n", total_cluster_length);
    printf("Largest Cluster Size: %d\n", largest_cluster);
    printf("Average Cluster Length: %.2f\n", cluster_count ? (double)total_cluster_length / cluster_count : 0.0);
}

