#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 20000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

volatile uint8_t
	VAR_ReceiverBufor[32],
	VAR_ReceiverBuforIndex,
	VAR_TransmitterBufor[32],
	VAR_TransmitterBuforIndex,
	VAR_LengthOfCMD,
	VAR_Flag_AnalizeCommand,
	VAR_ModBus_BytesNumExpected,
	VAR_Flag_ReceivedAnswer;

volatile uint8_t
	VAR_Flag_PIT0,
	VAR_Flag_PIT1,
	VAR_Flag_TCA_200ms,
	VAR_Flag_TCA_500ms;

volatile uint8_t
	VAR_button_pause,
	VAR_start;

void Fun_LED_Init(void);

extern void Fun_CLK_Init (void);

#endif /* MAIN_H_ */
