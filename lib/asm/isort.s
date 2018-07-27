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

void gen_sort(int* a, const size_t count, int (*pred)(const int, const int));

Parameters: pointer to contiguous memory, size in words, a predicate functor
that implements the sorting criteria.

Here is my C version of insertion sort that I used to reason this out:

 for(int* key = (first + 1); key != end; key++)
                while(key > first && *key < *(key - 1)) {
                	swap(key, key - 1);
                	--key;
                 }

------------------------------------------------------------------------------*/
asm_int_less:	
	cmp	r0, r1
	ite	ge
	movge	r0, #0
	movlt	r0, #1  /*better explicity put in case r0 hold  > 1 */
	bx	lr
	
asm_int_notless:	
	cmp	r0, r1
	ite	ge
	movge	r0, #1
	movlt	r0, #0  /*better explicity put in case r0 hold  > 1 */
	bx	lr


/*----------------------------------------------------------------------------*/  
gen_sort:
       	stmfd   sp!, {r4-r11}   /* we will need more registers for the loops */
        mov     r7, #0  /* outer loop */
        mov     r8, r1  /* inner loop copy of outer loop or key */
        mov     r9, #0  /* inner loop acting as key - 1 */
        cmp     r1, #0  /* quick exit if len variable is zero */
        it      eq      /* IF above statement is true, next line executes */
        beq     sort_end
for_loop:
	add     r7, r7, #1       /* init at 0, first time set to 1 is good */
        cmp     r7, r1        /* check for outer loop termination condition */
        beq     sort_end      /* we are done, sorting */
        mov     r8, r7         /* ELSE r8 saves r7 */
        b       while_loop
while_loop:
	cmp     r8, #0           /* test for loop termination */
        beq     for_loop
        mov     r9, r8       /* ELSE  r8 key, r9 key - 1 */
        sub     r9, r9, #1
        ldr     r4, [r0, r8, lsl #2]  /* r4:  array[key] */
        ldr     r5, [r0, r9, lsl #2]  /* swap temp1 */
        // compare needs to save context
        mov     r11, r2                 /* save the predicate before push*/
        stmfd	sp!, {r0-r3, lr}
        mov	r0, r4
        mov	r1, r5
        blx     r11
	mov	r10, r0
	ldmfd	sp!, {r0-r3, lr}
        cmp     r10, #1
        bne     for_loop
        str     r4, [r0, r9, lsl #2]  /* ELSE swap*/
        str     r5, [r0, r8, lsl #2]
        sub     r8, r8, #1  /* and continue */
        b       while_loop
sort_end:
        ldmfd   sp!, {r4-r11}         /* restore registers used for the loops */
	bx	lr


