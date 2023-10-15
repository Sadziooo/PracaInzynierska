#include <avr/io.h>

.global Fun_CLK_Init

Fun_CLK_Init:
	push R25
	push R30
	push R31
	push R24

	LDI R25, 0		; settings
	LDI R30, 0x61	; Low address ControlB
	LDI R31, 0x00	; High address ControlB

	//ccp
	LDI R24, 0xD8	; password
	OUT 0x34, R24
	//ctrlB
	STD Z+0, R25

	pop R24
	pop R31
	pop R30
	pop R25
	ret

