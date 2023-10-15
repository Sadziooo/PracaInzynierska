#include "USART_data.h"

void Fun_USART_Init(void)
{
	PORTB.DIRCLR = PIN3_bm;
	PORTB.DIRSET = PIN2_bm;
	
	USART0.BAUD = (uint16_t)((F_CPU * 64L) / (16L * BAUD_RATE));
	
	USART0.CTRLA = 1 << USART_RXCIE_bp			/* Receive Complete Interrupt Enable: enabled */
				 | 0 << USART_TXCIE_bp			/* Transmit Complete Interrupt Enable: disabled */
				 | 0 << USART_DREIE_bp			/* Data Register Empty Interrupt Enable: disabled */
				 | 0 << USART_RXSIE_bp			/* Receiver Start Frame Interrupt Enable: disabled */
				 | 0 << USART_LBME_bp			/* Loop-back Mode Enable: disabled */
				 | 0 << USART_ABEIE_bp			/* Auto-baud Error Interrupt Enable: disabled */
				 | 0 << USART_RS4850_bp;		/* RS-485 Mode: disabled */
	
	USART0.CTRLB = 0 << USART_MPCM_bp			/* Multi-processor Communication Mode: disabled */
				 | 0 << USART_ODME_bp			/* Open Drain Mode Enable: disabled */
				 | 1 << USART_RXEN_bp			/* Receiver Enable: enabled */
				 | USART_RXMODE_NORMAL_gc		/* Normal mode */
				 | 0 << USART_SFDEN_bp			/* Start Frame Detection Enable: disabled */
				 | 1 << USART_TXEN_bp;			/* Transmitter Enable: enabled */
	
	USART0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc	/* Asynchronous Mode */
				 | USART_CHSIZE_8BIT_gc			/* Character size: 8 bit */
				 | USART_PMODE_DISABLED_gc		/* No Parity */
				 | USART_SBMODE_1BIT_gc;		/* 1 stop bit */
}

void USART0_sendFrame(uint8_t SlaveID, uint8_t FunCode, uint16_t OutAddr, uint16_t OutVal, uint16_t CRC) // in HEX
{
	VAR_TransmitterBufor[0] = SlaveID;
	VAR_TransmitterBufor[1] = FunCode;
	VAR_TransmitterBufor[2] = (OutAddr >> 8);
	VAR_TransmitterBufor[3] = OutAddr;
	VAR_TransmitterBufor[4] = (OutVal >> 8);
	VAR_TransmitterBufor[5] = OutVal;
	VAR_TransmitterBufor[6] = CRC;
	VAR_TransmitterBufor[7] = (CRC >> 8);
	
	VAR_TransmitterBuforIndex = 1;
	VAR_LengthOfCMD = 8;
	VAR_ModBus_BytesNumExpected = 8;
	VAR_ReceiverBuforIndex = 0;
	
	USART0.TXDATAL = VAR_TransmitterBufor[0];
	USART0.CTRLA |= (1 << USART_DREIE_bp);
}

/*
UNfortunately, Modbus uses a CRC16 calculation as follows:
The Cyclical Redundancy Check (CRC) field is two bytes, containing a 16–bit binary value. The CRC value is calculated by the transmitting device, which appends the CRC to the message. 
The receiving device recalculates a CRC during receipt of the message, and compares the calculated value to the actual value it received in the CRC field. 
If the two values are not equal, an error results.

The CRC is started by first preloading a 16–bit register to all 1’s. Then a process begins of applying successive 8–bit bytes of the message to the current contents of the register. 
Only the eight bits of data in each character are used for generating the CRC. Start and stop bits, and the parity bit, do not apply to the CRC.

During generation of the CRC, each 8–bit character is exclusive ORed with the register contents. Then the result is shifted in the direction of the least significant bit (LSB), 
with a zero filled into the most significant bit (MSB) position. The LSB is extracted and examined. If the LSB was a 1, the register is then exclusive ORed with a preset, fixed value. 
If the LSB was a 0, no exclusive OR takes place. This process is repeated until eight shifts have been performed. After the last (eighth) shift, 
the next 8–bit character is exclusive ORed with the register’s current value, and the process repeats for eight more shifts as described above.

The final contents of the register, after all the characters of the message have been applied, is the CRC value.

A procedure for generating a CRC is:

1. Load a 16–bit register with FFFF hex (all 1’s). Call this the CRC register.

2. Exclusive OR the first 8–bit byte of the message with the low–order byte of the 16–bit CRC register, putting the result in the CRC register.

3. Shift the CRC register one bit to the right (toward the LSB), zero–filling the MSB. Extract and examine the LSB.

4. (If the LSB was 0): Repeat Step 3 (another shift). (If the LSB was 1): Exclusive OR the CRC register with the polynomial value A001 hex (1010 0000 0000 0001).

5. Repeat Steps 3 and 4 until 8 shifts have been performed. When this is done, a complete 8–bit byte will have been processed.
*/