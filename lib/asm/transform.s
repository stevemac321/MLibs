/*-----------------------------------------------------------------------------
transform.s
This file is distributed under the GNU General Public License, version 2 (GPLv2)
See https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
Author: Stephen E. MacKenzie
------------------------------------------------------------------------------*/

        .syntax unified
        .text
	.thumb_func
        .global transform
	.global rand_wrapper

/* -----------------------------------------------------------------------------
void transform(int* a, const size_t count, int (*pred)(int));

Parameters: pointer to integer array, number of elements, function pointer that
modifies the elements.  Example, the following populates the array with random
values.

transform(a, _countof(a), rand_wrapper);  
------------------------------------------------------------------------------*/
transform:
        stmfd   sp!, {r4-r9}  /* need more vars*/
        mov     r9, #0  /* loop counter */
        cmp     r1, #0  /* quick exit if len variable is zero */
        it      eq      /* IF above statement is true, next line executes */
        moveq   pc, lr  /* return, exit function */
	mov	r6, r0	/* save array */
	mov	r7, r1  /* save len*/
	mov	r8, r2	/*save functor */
for_loop:
        stmfd   sp!, {r0-r3, lr}  /* push context*/
        blx	r8
        mov     r4, r0            /* save return */
        ldmfd   sp!, {r0-r3, lr}  /* pop context*/
        str     r4, [r0, r9, lsl #2]  /* write random number to sub r3 */
        add     r9, r9, #1
        cmp     r9, r1                  /* loop termination condition */
        bne     for_loop
done:
        ldmfd   sp!, {r4-r9}  /* pop back those vars */
        bx	lr

rand_wrapper:
	stmfd	sp!, {r4-r5}
	stmfd	sp!, {r0-r3, lr}
	bl	rand
	mov 	r4, r0
	lsr	r4, r4, #20
	ldmfd	sp!, {r0-r3, lr}
	mov	r0, r4
	ldmfd	sp!, {r4-r5}
	bx	lr

