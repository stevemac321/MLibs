/*-----------------------------------------------------------------------------
icopy.s 
This file is distributed under the GNU General Public License, version 2 (GPLv2)
See https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
Author: Stephen E. MacKenzie
------------------------------------------------------------------------------*/

        .syntax unified
        .text
	.thumb_func
        .global icopy

/* -----------------------------------------------------------------------------
C declaration:  
void icopy(int* restrict array1, int* restrict array2, const size_t len); 

Parameters: pointer to a destination buffer, pointer to a source integer array, 
size of the source array. 

Requirements: 
1.) the destination buffer must be large enough as to not overflow
2.) if the destination and source overlap, the results are undefined.
	r0 destingation
	r1 source
	r2 len
------------------------------------------------------------------------------*/
icopy:  
        stmfd   sp!, {r4-r9}   /* we will need more registers for the loops */
        mov     r9, #0  /* loop counter*/
        cmp     r2, #0  /* quick exit if len variable is zero */
	        it      eq      /* IF above statement is true, next line executes */
	
        moveq   pc, lr  /* return, exit function */

for_loop:
        ldr     r4, [r1, r9, lsl #2]
        str     r4, [r0, r9, lsl #2]
        add     r9, r9, #1
        cmp     r9, r2     /* loop termination condition */
        bne     for_loop

        ldmfd   sp!, {r4-r9}  
        bx      lr

