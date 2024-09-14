/*-----------------------------------------------------------------------------
isequal.s 
This file is distributed under the GNU General Public License, version 2 (GPLv2)
See https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
Author: Stephen E. MacKenzie
------------------------------------------------------------------------------*/

        .syntax unified
        .text
	.thumb_func
        .global is_equal

/* -----------------------------------------------------------------------------
Compares two array ranges of len by value.

C declaration:  
bool isequal(int* array, const size_t len) 

Parameters: pointer to integer array, number of elements

Returns true or false.
------------------------------------------------------------------------------*/
is_equal:  
        stmfd   sp!, {r4-r9}   /* we will need more registers for the loops */
        mov     r9, #0          /* counter*/
/* r0=s1, r1=s2, len=r2*/
for_loop:
        cmp 	r9, r2
	itt 	eq
	moveq	r0, #1
	beq	is_equal_done

        ldr     r4, [r0, r9, lsl #2]  /* get next s1 element */
        ldr     r5, [r1, r9, lsl #2]  /* get next s2 element */   
        cmp     r4, r5
	itt	ne
	movne	r0, #0
	bne	is_equal_done
	
        add     r9,r9,#1
        b       for_loop
is_equal_done:	
        ldmfd   sp!, {r4-r9}         /* restore */
	bx 	lr                      /* return */


