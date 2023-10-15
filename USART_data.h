#ifndef UART_DATA_H_
#define UART_DATA_H_

#include "main.h"

#define BAUD_RATE 9600
//#define USART0_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)
#define MAX_COMMAND_LEN		8
#define INIT_DELAY			10	/* Delay to invalidate the after-reset noise on the PB2 pin (TX) */
 
 volatile uint8_t
	 VAR_ReceiverBufor[32],
	 VAR_ReceiverBuforIndex,
	 VAR_TransmitterBufor[32],
	 VAR_TransmitterBuforIndex,
	 VAR_LengthOfCMD,
	 VAR_Flag_AnalizeCommand,
	 VAR_ModBus_BytesNumExpected,
	 VAR_Flag_ReceivedAnswer;
 
void Fun_USART_Init(void);

uint8_t USART0_readChar(void);

void USART0_readFrame(void);

void USART0_sendChar(unsigned char c);

void USART0_sendString(char *str);

void USART0_sendFrame(uint8_t SlaveID, uint8_t FunCode, uint16_t OutAddr, uint16_t OutVal, uint16_t CRC); // | Slave ID (8b) | Fun Code (8b) | write address (16b) | write value (16b) | Suma kontrolna CRC ( 16b ) | 

void executeCommand(char *command);

uint16_t CRC16 (const uint8_t *nData, uint16_t wLength);

#endif /* UART_DATA_H_ */