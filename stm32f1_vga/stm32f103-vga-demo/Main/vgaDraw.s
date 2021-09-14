//
.syntax unified

@ Arguments:
@  r1  pixels
@  r2  colors
@  r3  destination
.section	.text.vgaDraw,"ax",%progbits
//.section .ramcode,ax,%progbits
.balign 4
.global vgaDraw
.thumb_func
vgaDraw:
	@ parameters
	pix   .req r0
	col   .req r1
	odr   .req r2

	push {r4, r5, r6}

	ldr r5, [pix], #4  // pixels for characters 0..3
	ldr r3, [col], #4  // colors for characters 0..1

	// character #0
	  ror r5, r5, #4     // pixels >> 4
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 7
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr

	// character #1
	  ror r3, r3, #16    // colors
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 5
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ldr r3, [col], #4  // colors for next 2 characters
	  strb r4, [odr]

	// character #2
	  nop
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 7
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr

	// character #3
	  ror r3, r3, #16    // colors
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 3
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  ldr r6, [pix], #4  // r6: pixels for next 4 characters
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ldr r3, [col], #4  // colors for next 2 characters
	  strb r4, [odr]

	// character #4
	  mov r5, r6         // pixels
	  ror r5, r5, #4     // pixels >> 4
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 7
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr

	// character #5
	  ror r3, r3, #16    // colors
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 5
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ldr r3, [col], #4  // colors for next 2 characters
	  strb r4, [odr]

	// character #6
	  nop
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 7
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr

	// character #7
	  ror r3, r3, #16    // colors
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 3
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  ldr r6, [pix], #4  // r6: pixels for next 4 characters
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ldr r3, [col], #4  // colors for next 2 characters
	  strb r4, [odr]

	// character #8
	  mov r5, r6         // pixels
	  ror r5, r5, #4     // pixels >> 4
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 7
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr

	// character #9
	  ror r3, r3, #16    // colors
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 5
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ldr r3, [col], #4  // colors for next 2 characters
	  strb r4, [odr]

	// character #10
	  nop
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 7
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr

	// character #11
	  ror r3, r3, #16    // colors
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 3
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  ldr r6, [pix], #4  // r6: pixels for next 4 characters
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ldr r3, [col], #4  // colors for next 2 characters
	  strb r4, [odr]

	// character #12
	  mov r5, r6         // pixels
	  ror r5, r5, #4     // pixels >> 4
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 7
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr

	// character #13
	  ror r3, r3, #16    // colors
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 5
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ldr r3, [col], #4  // colors for next 2 characters
	  strb r4, [odr]

	// character #14
	  nop
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 7
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr

	// character #15
	  ror r3, r3, #16    // colors
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 3
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  ldr r6, [pix], #4  // r6: pixels for next 4 characters
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ldr r3, [col], #4  // colors for next 2 characters
	  strb r4, [odr]

	// character #16
	  mov r5, r6         // pixels
	  ror r5, r5, #4     // pixels >> 4
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 7
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr

	// character #17
	  ror r3, r3, #16    // colors
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 5
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ldr r3, [col], #4  // colors for next 2 characters
	  strb r4, [odr]

	// character #18
	  nop
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 7
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr

	// character #19
	  ror r3, r3, #16    // colors
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 3
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  ldr r6, [pix], #4  // r6: pixels for next 4 characters
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ldr r3, [col], #4  // colors for next 2 characters
	  strb r4, [odr]

	// character #20
	  mov r5, r6         // pixels
	  ror r5, r5, #4     // pixels >> 4
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 7
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr

	// character #21
	  ror r3, r3, #16    // colors
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 5
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ldr r3, [col], #4  // colors for next 2 characters
	  strb r4, [odr]

	// character #22
	  nop
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 7
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr

	// character #23
	  ror r3, r3, #16    // colors
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 3
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  ldr r6, [pix], #4  // r6: pixels for next 4 characters
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ldr r3, [col], #4  // colors for next 2 characters
	  strb r4, [odr]

	// character #24
	  mov r5, r6         // pixels
	  ror r5, r5, #4     // pixels >> 4
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 7
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr

	// character #25
	  ror r3, r3, #16    // colors
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 5
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ldr r3, [col], #4  // colors for next 2 characters
	  strb r4, [odr]

	// character #26
	  nop
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 7
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr

	// character #27
	  ror r3, r3, #16    // colors
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 3
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  ldr r6, [pix], #4  // r6: pixels for next 4 characters
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ldr r3, [col], #4  // colors for next 2 characters
	  strb r4, [odr]

	// character #28
	  mov r5, r6         // pixels
	  ror r5, r5, #4     // pixels >> 4
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 7
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr

	// character #29
	  ror r3, r3, #16    // colors
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 5
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ldr r3, [col], #4  // colors for next 2 characters
	  strb r4, [odr]

	// character #30
	  nop
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 7
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr

	// character #31
	  ror r3, r3, #16    // colors
	  ror r5, r5, #15    // pixels >> 15
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.rept 3
	  nop
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	.endr
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  ldr r6, [pix], #4  // r6: pixels for next 4 characters
	  and r4, r5, #8
	  lsr r4, r3, r4
	  strb r4, [odr]
	  ror r5, r5, #31    // pixels << 1
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ror r5, r5, #31    // pixels << 1
	  strb r4, [odr]
	  and r4, r5, #8
	  lsr r4, r3, r4
	  ldr r3, [col], #4  // colors for next 2 characters
	  strb r4, [odr]

	// 0 at the end
	  mov r4, #0
	  nop
	  nop
	  nop
	  strb r4, [odr]

	pop	{r4, r5, r6}
	bx lr
