#include "main.h"
#include "Timers.h"
#include "Debouncer.h"
#include "USART_data.h"
#include "LED_Sterowanie.h"

#define def_CRCstart		(VAR_ReceiverBufor[6] != 0xA5) && (VAR_ReceiverBufor[7] != 0xFA)
#define def_CRCreset		(VAR_ReceiverBufor[6] != 0xF4) && (VAR_ReceiverBufor[7] != 0x3A)
#define def_CRCpauseON		(VAR_ReceiverBufor[6] != 0x04) && (VAR_ReceiverBufor[7] != 0x3A)
#define def_CRCpauseOFF		(VAR_ReceiverBufor[6] != 0x14) && (VAR_ReceiverBufor[7] != 0x0A)
#define def_CRCpauseOFFsrc	(VAR_ReceiverBufor[6] != 0x45) && (VAR_ReceiverBufor[7] != 0xCA)

volatile uint8_t
	VAR_ReceiverBufor[32],
	VAR_ReceiverBuforIndex = 0,
	VAR_TransmitterBufor[32],
	VAR_TransmitterBuforIndex = 0,
	VAR_LengthOfCMD = 0,
	VAR_Flag_AnalizeCommand = false,
	VAR_ModBus_BytesNumExpected = 0,
	VAR_Flag_ReceivedAnswer = false;

volatile uint8_t
	VAR_Flag_PIT0 = false,
	VAR_Flag_PIT1 = false,
	VAR_Flag_TCA_200ms = false,
	VAR_Flag_TCA_500ms = false;
	
volatile uint8_t
	VAR_button_pause = 0,
	VAR_start = 0;

extern void Fun_CLK_Init (void);

int main(void)
{
	/* Initialization section */
	Fun_CLK_Init();
	Fun_LED_Init();
	Fun_PIT_Init();
	Fun_USART_Init();
	Fun_TCA0_Init();
	
	VAR_button_pause = 0;
	
	sei();
	
	while (1)
	{
		// 0x01, 0x05, 0x8000, 0xFF00, 0xFAA5 - write 1
		// 0x01, 0x05, 0x8000, 0x0000, 0x0AE4 - write 0
		
		if (button0_down == 1)
		{
			USART0_sendFrame(0x01, 0x05, 0x8000, 0xFF00, 0xFAA5);	// start button
			VAR_start = 1;
			button0_down = 0;
		}
		
		if (button1_down == 1)
		{
			USART0_sendFrame(0x01, 0x05, 0x8001, 0xFF00, 0x3AF4);	// reset button
			button1_down = 0;
		}
		
		if ((button2_down == 1) && (VAR_button_pause == 0))
		{
			USART0_sendFrame(0x01, 0x05, 0x8002, 0xFF00, 0x3A04);	// pause button ON
			VAR_button_pause++;
			button2_down = 0;
		}
		
		if((button2_down == 1) && (VAR_button_pause == 1))
		{
			USART0_sendFrame(0x01, 0x05, 0x8002, 0x0000, 0xCA45);	// pause button OFF
			VAR_button_pause++;
			button2_down = 0;
		}
		
		if((button2_down == 1) && (VAR_button_pause == 2))
		{
			USART0_sendFrame(0x01, 0x05, 0x8003, 0x0000, 0x0A14);	// pause button OFF SRC
			VAR_button_pause = 0;
			button2_down = 0;
		}
		
		if (VAR_start == 1)
		{																// odpytywanie 21 coilow od 8010 do 801C
			if(VAR_Flag_PIT0)											// response to 01 01 03 xx yy zz CRC, gdzie:
			{															// {0 1 2 [3] [4] [5] 6 7} interesuj¹ nas
				USART0_sendFrame(0x01, 0x01, 0x8010, 0x0015, 0xC0D5);	// komórki 3, 4 i 5 
																		// xx to coile 8017-8010
				VAR_Flag_PIT0 = false;									// yy to coile 801F-8018
			}															// zz to coile 8027-8020, 
																		// w tym 8027-8025 to bity uzupe³niaj¹ce
																		
			if (def_CRCstart && def_CRCreset && def_CRCpauseON && def_CRCpauseOFF && def_CRCpauseOFFsrc)
			{
				Fun_CoilsChecking();
			}
		}
		
	} // koniec while
	
} // koniec main

uint16_t
	LocVAR_x = 0,
	LocVAR_y = 0;
uint8_t
	LocVAR_z = 0;
uint8_t
	LocVAR_a = 0;

ISR (TCA0_OVF_vect) // 1 milisekunda
{
	LocVAR_z++;
	
	if (LocVAR_z == 12)
	{
		debounce0(BUTTON0_PIN, BUTTON0_MASK, BUTTON0_PULLUP);	// start
		debounce1(BUTTON1_PIN, BUTTON1_MASK, BUTTON1_PULLUP);	// reset
		debounce2(BUTTON2_PIN, BUTTON2_MASK, BUTTON2_PULLUP);	// pause
			
		LocVAR_z = 0;
	}

	
	LocVAR_x++; LocVAR_y++;
	
	if (LocVAR_x == 200)
	{
		VAR_Flag_TCA_200ms = true;
		LocVAR_x = 0;
	}
	
	if (LocVAR_y == 500)
	{
		VAR_Flag_TCA_500ms = true;
		
		LocVAR_y = 0;
	}
	
	
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

ISR (RTC_PIT_vect) // 0.5 sekundy
{
	RTC.PITINTFLAGS = RTC_PI_bm;
	
	LocVAR_a++;
	
	if (LocVAR_a == 1)
	{
		VAR_Flag_PIT0 = true;
	}
	
	if (LocVAR_a == 2)
	{ 
		VAR_Flag_PIT1 = true;
		
		LocVAR_a = 0;
	}
}

ISR (USART0_DRE_vect)
{	
	if (VAR_TransmitterBuforIndex < VAR_LengthOfCMD) 
	{
		
		USART0.TXDATAL = VAR_TransmitterBufor[VAR_TransmitterBuforIndex++];
	}
	else
	{
		USART0.CTRLA &= ~(1 << USART_DREIE_bp);
	}
}

ISR (USART0_RXC_vect)
{
	VAR_ReceiverBufor[VAR_ReceiverBuforIndex++] = USART0.RXDATAL;
	if(VAR_ReceiverBuforIndex == VAR_ModBus_BytesNumExpected)
	{
		VAR_Flag_ReceivedAnswer = true;
	}
}
