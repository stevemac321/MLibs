/*-----------------------------------------------------------------------------
spinlock.s GNU version
Technique learned from Valvano: Real-Time Operating Systems for ARM Cortex 
Microcontrollers.

void spinlock_wait(int *count) ;
void spinlock_signal(int *count) ;
------------------------------------------------------------------------------*/

	.syntax unified
        .text
	.thumb_func
        .global spinlock_wait
	.global spinlock_signal

spinlock_wait
	ldrex	r1, [r0] 	/* counter */
	subs	r1, #1	 	/* decrement owner count, set PSR status flag*/
	itt	pl	 	/*IF, THEN, THEN block*/
	strexpl	r2, r1, [r0] 	/* THEN try to update counter */
	cmppl	r2, #0		/* THEN if refcount zero (resource available) */

	bne	spinlock_wait	/* ELSE != 0, try again */

	bx	lr		/*if acquired, return */
	
spinlock_signal
	ldrex	r1, [r0]	/* counter */
	add	r1, #1		/* increment owner count */
	strex	r2, r1, [r0]	/* try to update */
	cmp	r2, #0		/*check for success */
	bne	spinlock_signal	/* if no success, try again */

	bx	lr		/* else if success, return */
