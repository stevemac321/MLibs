/*==============================================================================
 Name        : bitmanip.inl
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>


uint32_t bit_set(uint32_t v, const size_t pos) {
  assert(v && pos < sizeof(uint32_t) * 8);
  v |= 1 << pos;
	return v;
}
bool bit_lookup(const uint32_t v, const size_t find) {
  assert(find < sizeof(uint32_t) * 8);
  return ((v >> find) & 1);
}

void bit_print(const uint32_t v) {
  assert(v);
  size_t len = sizeof(uint32_t) * 8;

  for (size_t pos = 0; pos < len; pos++)
    printf("%d", bit_lookup(v, pos));
}

void print_bin(const int x) {
  size_t len = sizeof(int) * 8;
  for(size_t pos=0; pos< len; pos++)
    printf("%d", (x >> pos) & 1);

  puts("\n");
    
}
size_t bit_count(const uint32_t x) {
  assert(x);
  size_t count = 0;
  uint32_t tmp = x;

  while(tmp) {
    if(tmp & 1)
      count++;

    tmp >>= 1;
  }
  return count;
}

/*
Setting a bit
Use the bitwise OR operator (|) to set a bit.

number |= 1 << x;
That will set bit x.

Clearing a bit
Use the bitwise AND operator (&) to clear a bit.

number &= ~(1 << x);
That will clear bit x. You must invert the bit string with the bitwise NOT
operator (~), then AND it.

Toggling a bit
The XOR operator (^) can be used to toggle a bit.

number ^= 1 << x;
That will toggle bit x.

Checking a bit
To check a bit, shift the number x to the right, then bitwise AND it:

bit = (number >> x) & 1;
That will put the value of bit x into the variable bit.

Changing the nth bit to x
Setting the nth bit to either 1 or 0 can be achieved with the following:

number ^= (-x ^ number) & (1 << n);
 */
