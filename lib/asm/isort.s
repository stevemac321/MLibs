/*-----------------------------------------------------------------------------
isort.s - assembly insertion sort for education purposes
This file is distributed under the GNU General Public License, version 2 (GPLv2)
See https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
Author: Stephen E. MacKenzie
------------------------------------------------------------------------------*/

        .syntax unified
        .text
	.thumb_func
        .global gen_sort
	.global asm_int_less
	.global asm_int_notless

/* -----------------------------------------------------------------------------
C declaration:  

void gen_sort(int *a, const size_t count, int (*pred)(const int, const int));

Parameters: pointer to contiguous memory, size in words, a compare functor
that implements the sorting criteria. Compare returns 1 is arg1 > arg2.

Here is my C version of insertion sort that I used to reason this out:

 for(int* key = (first + 1); key != end; key++)
                while(key > first && *key < *(key - 1)) {
                	swap(key, key - 1);
                	--key;
                 }

What it does:  The outer loop iterates through each element starting at a + 1.
a + 1 is "current".  

Then the inner loop compares "current" to  "current - 1".  "Current" 
swaps left (decrementing) until it is no longer less that current - 1 (or until 
it reaches the start of the array limit, which means it is the lowest so far). 
Meeting that condition means that "current" has found its proper place so far
in the sorted partition (which is from start to current).  The inner loop 
returns control to the outer loop.

The outer loop increments and becomes the next "current."  The outer loop
terminates once it has iterated all of the elements.  The sort is done.

gen_sort will sort any contiguous memory whose elements are the same size as a
pointer on this platform, int, uint32_t, const char *, any *, etc.  The user
supplied compare function needs to be aware of this.  see str_cmp for example.
------------------------------------------------------------------------------*/
asm_int_less:	
	cmp	r0, r1
	ite	ge
	movge	r0, #0 /* greater than or equal returns 0. (false) */
	movlt	r0, #1  /* less that returns 1. (true) */
	bx	lr
	
asm_int_notless:	
	cmp	r0, r1
	ite	ge
	movge	r0, #1 /* greater than or equal returns 1 (true) */
	movlt	r0, #0  /* less than returns 0 (false) */
	bx	lr


/*----------------------------------------------------------------------------*/  
gen_sort:
       	stmfd   sp!, {r4-r11}   /* we will need more registers for the loops */
        mov     r7, #0  /* initialize  */
        mov     r8, r1  /* move array count into r8 */
        mov     r9, #0  /* initialize  */
        cmp     r1, #0  /* quick exit if count arg is zero */
        it      eq      /* IF above statement is true, next line executes */
        beq     sort_end
for_loop:
        /* outer loop counter increments */
	add     r7, r7, #1    /* r7 is used for outer loop incr counter */
        cmp     r7, r1        /* is outer loop counter == to count arg? */
        beq     sort_end      /* if so, we are done, sorting */
        mov     r8, r7        /* ELSE r8 saves r7 */
        b       while_loop
while_loop:
        /* inner loop counter decrements, termination cond is zero. */
	cmp     r8, #0      /* test for loop termination */
        beq     for_loop    /* break out of inner loop if equal */
        mov     r9, r8      /* r9 = r8, which was incr end of last iteration */
        sub     r9, r9, #1  /* r8 is current index, r9 current - 1 */

        /* mov the value of the array(r0) at offset r8<<2 into r4 */
        ldr     r4, [r0, r8, lsl #2]  /* r0 + r8<<2 is the effective addr */

        /* mov the value of the array(r0) + offset r9<<2 into r5 */
        ldr     r5, [r0, r9, lsl #2] /* r0 + r9<<2 is the effective addr*/ 

        mov     r11, r2  /* save the user compare before saving context */
        stmfd	sp!, {r0-r3, lr}  /* save context before branching */

        /* preparing args for branch to user provided compare. */
        mov	r0, r4  /* mov first arg into r0. */
        mov	r1, r5  /* mov second arg into r1. */
        blx     r11  /* branch to user supplied compare function */

        /* mov compare return from  r0 into r10 before restoring prior context */
	mov	r10, r0  
	ldmfd	sp!, {r0-r3, lr} /* restore context */
        
        /* compare the return of the compare function.  Is the first element
         * great than the second?  */
        cmp     r10, #1  

        /* if the first element was not greater than the second, then it has
         found its proper place in the sorted partition. (start of current  */
        bne     for_loop  /* break out of loop if not greater than */
        str     r4, [r0, r9, lsl #2]  /* ELSE swap*/
        str     r5, [r0, r8, lsl #2]
        sub     r8, r8, #1  /* and continue */
        b       while_loop
sort_end:
        ldmfd   sp!, {r4-r11}         /* restore registers used for the loops */
	bx	lr

/* LSL #2 Syntax  

        (mov the value of the array(r0) at offset r8<<2 into r4)

        ldr     r4, [r0, r8, lsl #2]  (r0 + r8<<2 is the effect addr.)
        
        (the lsl #2 aligns the offset on 32 bit boundary.  Consider this C code:

        #include <stdio.h>
        #include <stdint.h>

        int main()
        {
                // base is  r0, offset is r8.
                uint32_t base = 0x20017F08; (val taken from debugging session). 

                for(uint32_t offset = 0; offset < 10; offset++) {
                        uint32_t addr = base + (offset << 2);
                        printf("effective addr: %x addr - base: %d\n", 
                                                   addr, addr - base);
                }
        }

        prints: 
        effective address: 20017f08 address - base: 0
        effective address: 20017f0c address - base: 4
        effective address: 20017f10 address - base: 8
        effective address: 20017f14 address - base: 12
        effective address: 20017f18 address - base: 16
        effective address: 20017f1c address - base: 20
        effective address: 20017f20 address - base: 24
        effective address: 20017f24 address - base: 28
        effective address: 20017f28 address - base: 32
        effective address: 20017f2c address - base: 36

*/
