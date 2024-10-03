/**
 * @page bitmanip Bit Manipulation Utilities
 * @brief Overview of the bit manipulation functions provided in `bitmanip.inl`.
 *
 * ## Bit Manipulation Utilities Overview
 *
 * The `bitmanip` module provides a set of utility functions for working with individual bits in a `uint32_t` data type. These functions are designed to facilitate bitwise operations commonly needed in low-level programming, such as setting, clearing, toggling, and checking specific bits within a word.
 *
 * ### Key Features
 * - **Setting, Clearing, and Toggling Bits**: Functions like `bit_set`, `bit_clear`, and `bit_toggle` allow for easy manipulation of individual bits within a 32-bit integer.
 * - **Bit Lookup**: The `bit_lookup` function returns whether a specific bit is set or cleared.
 * - **Counting Set Bits**: The `bit_count` function counts the number of bits set to 1 in a given integer.
 * - **Binary Representation**: The `bit_print` and `print_bin` functions provide a visual representation of the bits in a `uint32_t` value, which is useful for debugging and verification.
 *
 * ### Major Functions
 * - **`uint32_t bit_set(uint32_t v, size_t pos)`**: Sets the bit at position `pos` in `v` to 1.
 * - **`bool bit_lookup(uint32_t v, size_t pos)`**: Returns whether the bit at position `pos` is set (1) or cleared (0).
 * - **`void bit_print(uint32_t v)`**: Prints the binary representation of `v` to the console.
 * - **`size_t bit_count(uint32_t x)`**: Counts the number of bits set to 1 in `x`.
 *
 * ### Usage Example
 *
 * ```c
 * uint32_t value = 0;
 * value = bit_set(value, 3);  // Sets the 3rd bit of `value` to 1
 * bool isSet = bit_lookup(value, 3);  // Returns true since the 3rd bit is set
 * bit_print(value);  // Outputs the binary representation of `value`
 * size_t count = bit_count(value);  // Counts the number of set bits in `value`
 * ```
 *
 * These utilities are particularly useful in embedded systems programming and low-level software development where precise control over individual bits is required. For more details on each function, refer to the specific implementation in `bitmanip.inl`.
 */


### Bit Manipulation Utility (`bitmanip.inl`) Overview

**Purpose**:  
The `bitmanip.inl` file is a collection of inline functions designed to perform low-level bit manipulation operations on 32-bit integers. These functions are useful for embedded systems, graphics, and algorithms that require efficient bit-level operations.

### Key Functions and Their Descriptions

1. **`bit_set`**:  
   Sets a specific bit at position `pos` in a given 32-bit integer `v`. This function uses the bitwise OR (`|`) operation to set the bit and returns the modified integer.

   ```c
   uint32_t bit_set(uint32_t v, const size_t pos);
   ```
   - **Parameters**:  
     - `v`: The original integer in which the bit is to be set.
     - `pos`: The position of the bit to be set (0-based index).
   - **Returns**:  
     The modified integer with the bit at `pos` set to `1`.

   - **Example**:
     ```c
     uint32_t result = bit_set(0x0000, 3);  // Sets the 3rd bit of 0x0000, returns 0x0008
     ```

2. **`bit_lookup`**:  
   Checks if a specific bit is set in a given 32-bit integer `v` at position `find`. It shifts the integer to the right by `find` positions and checks the least significant bit using a bitwise AND (`&`).

   ```c
   bool bit_lookup(const uint32_t v, const size_t find);
   ```
   - **Parameters**:  
     - `v`: The integer to check.
     - `find`: The position of the bit to check.
   - **Returns**:  
     `true` if the bit at `find` is set (`1`), `false` otherwise.

   - **Example**:
     ```c
     bool isSet = bit_lookup(0x0008, 3);  // Checks if the 3rd bit is set, returns true
     ```

3. **`bit_print`**:  
   Prints the binary representation of a 32-bit integer. This function iterates through each bit and prints it using `bit_lookup` to check its value.

   ```c
   void bit_print(const uint32_t v);
   ```
   - **Parameters**:  
     - `v`: The integer whose binary representation will be printed.

   - **Example**:
     ```c
     bit_print(0x0008);  // Prints "00000000000000000000000000001000"
     ```

4. **`print_bin`**:  
   Prints the binary representation of a given integer `x` using a direct approach. This function iterates through each bit and prints its value.

   ```c
   void print_bin(const int x);
   ```
   - **Parameters**:  
     - `x`: The integer whose binary representation will be printed.

   - **Example**:
     ```c
     print_bin(4);  // Prints "00000000000000000000000000000100"
     ```

5. **`bit_count`**:  
   Counts the number of set bits (`1`s) in a given 32-bit integer `x`. This function iteratively shifts the bits to the right and counts how many `1`s are present.

   ```c
   size_t bit_count(const uint32_t x);
   ```
   - **Parameters**:  
     - `x`: The integer to analyze.
   - **Returns**:  
     The number of set bits in the given integer.

   - **Example**:
     ```c
     size_t count = bit_count(0x000F);  // Returns 4 as there are four 1's in the binary representation
     ```

### Usage Examples

```c
int main() {
    uint32_t value = 0x0;
    value = bit_set(value, 3);  // Set the 3rd bit
    bit_print(value);  // Prints: 00000000000000000000000000001000

    bool is_set = bit_lookup(value, 3);  // Returns true
    printf("Is bit 3 set? %s\n", is_set ? "Yes" : "No");

    size_t count = bit_count(value);  // Returns 1
    printf("Number of set bits: %zu\n", count);

    return 0;
}
```

### Additional Bit Manipulation Techniques

1. **Setting a Bit**:  
   Use the bitwise OR (`|`) operation to set a specific bit.

   ```c
   number |= 1 << x;  // Sets the `x`th bit of `number`.
   ```

2. **Clearing a Bit**:  
   Use the bitwise AND (`&`) operation with the negated bitmask.

   ```c
   number &= ~(1 << x);  // Clears the `x`th bit of `number`.
   ```

3. **Toggling a Bit**:  
   Use the bitwise XOR (`^`) operation to toggle a specific bit.

   ```c
   number ^= 1 << x;  // Toggles the `x`th bit of `number`.
   ```

4. **Checking a Bit**:  
   To check if a specific bit is set, shift the number to the right and use the bitwise AND.

   ```c
   bit = (number >> x) & 1;  // Puts the value of the `x`th bit into `bit`.
   ```

5. **Changing the `n`th Bit to `x`**:  
   To set or clear a specific bit based on a condition.

   ```c
   number ^= (-x ^ number) & (1 << n);  // Sets the `n`th bit to `x`.
   ```

### Summary

The `bitmanip.inl` file provides fundamental bit manipulation operations for efficient low-level programming. The functions are written to handle common bit-level tasks like setting, clearing, toggling, and checking bits, and also include more complex operations like counting set bits and printing binary representations. These utilities are critical for applications in embedded systems, networking, graphics, and anywhere direct bit manipulation is necessary for performance or resource constraints.