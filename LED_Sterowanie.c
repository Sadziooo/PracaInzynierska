#include "LED_Sterowanie.h"
#include "USART_data.h"
#include "Debouncer.h"

void Fun_LED_Init(void)
{
	PORTA.DIRSET = PIN7_bm | PIN6_bm | PIN5_bm | PIN4_bm | PIN3_bm | PIN2_bm | PIN1_bm;		// diody PA
	PORTB.DIRSET = PIN7_bm | PIN6_bm | PIN5_bm | PIN4_bm | PIN1_bm | PIN0_bm;				// diody PB
	PORTC.DIRSET = PIN5_bm | PIN4_bm | PIN3_bm;												// diody PC
	PORTC.DIRCLR = PIN2_bm | PIN1_bm | PIN0_bm;												// przyciski PC
	
	PORTA.OUTSET = PIN7_bm | PIN6_bm | PIN5_bm | PIN4_bm | PIN3_bm | PIN2_bm | PIN1_bm;
	PORTB.OUTSET = PIN7_bm | PIN6_bm | PIN5_bm | PIN4_bm | PIN1_bm | PIN0_bm;
	PORTC.OUTSET = PIN5_bm | PIN4_bm | PIN3_bm;
}

void Fun_CoilsChecking(void)
{
// --------------------------------------------------------------------------------------------------------------	
	
	if (def_zawor_1 && def_readcoil)	// zawor_1
		PORTA.OUTCLR = PIN1_bm;
	else
		PORTA.OUTSET = PIN1_bm;
 
	if (def_zawor_2 && def_readcoil)	// zawor_2
		PORTA.OUTCLR = PIN2_bm;
	else
		PORTA.OUTSET = PIN2_bm;

	if (def_zawor_3 && def_readcoil)	// zawor_3
		PORTA.OUTCLR = PIN3_bm;
	else
		PORTA.OUTSET = PIN3_bm;

	if (def_zawor_4 && def_readcoil)	// zawor_4
		PORTA.OUTCLR = PIN4_bm;
	else
		PORTA.OUTSET = PIN4_bm;

	if (def_zawor_5 && def_readcoil)	// zawor_5
		PORTA.OUTCLR = PIN5_bm;
	else
		PORTA.OUTSET = PIN5_bm;

	if (def_zawor_6 && def_readcoil)	// zawor_6
		PORTA.OUTCLR = PIN6_bm;
	else
		PORTA.OUTSET = PIN6_bm;

// --------------------------------------------------------------------------------------------------------------

	if (def_mieszadlo && def_readcoil)	// mieszadlo
		PORTA.OUTCLR = PIN7_bm;
	else
		PORTA.OUTSET = PIN7_bm;

	if (def_podajnik && def_readcoil)	// podajnik
		PORTC.OUTCLR = PIN3_bm;
	else
		PORTC.OUTSET = PIN3_bm;

// -------------------------------------------------------------------------------------------------------------

	if (def_error && def_readcoil)	// error
		PORTC.OUTCLR = PIN5_bm;
	else
		PORTC.OUTSET = PIN5_bm;
	
	while (def_pause && def_readcoil && !def_start) // pause
	{
		if(VAR_Flag_PIT0)
		{
			USART0_sendFrame(0x01, 0x01, 0x8010, 0x0015, 0xC0D5); 
			VAR_Flag_PIT0 = false; 
		}
		
		if (VAR_Flag_TCA_500ms == true)
		{
			PORTB.OUTTGL = PIN6_bm;
			VAR_Flag_TCA_500ms = false;
		}	
	}
	
	if (def_start && def_readcoil && !def_reset && !def_pause) // START
		PORTB.OUTCLR = PIN6_bm;
	else if (!def_start && def_readcoil && !def_reset && !def_pause)
		PORTB.OUTSET = PIN6_bm;
	
	while (def_reset && def_readcoil && !def_start) // reset
	{
		if(VAR_Flag_PIT0)
		{
			USART0_sendFrame(0x01, 0x01, 0x8010, 0x0015, 0xC0D5);
			VAR_Flag_PIT0 = false;
		}
		
		PORTA.OUTCLR = PIN6_bm | PIN5_bm | PIN4_bm | PIN3_bm;
		PORTA.OUTSET = PIN2_bm | PIN1_bm;
		
		if (VAR_Flag_TCA_500ms == true)
		{
			if (!def_ZB1_git && !def_ZB1_0)
			{
				PORTB.OUTTGL = PIN0_bm;
			}
			else if (def_ZB1_0)
			{
				PORTB.OUTSET = PIN0_bm;
			}
			
			if (!def_ZB2_git && !def_ZB2_0)
			{
				PORTB.OUTTGL = PIN1_bm;
			}
			else if (def_ZB2_0)
			{
				PORTB.OUTSET = PIN1_bm;
			}
			
			if (!def_ZB3_git && !def_ZB3_0)
			{
				PORTB.OUTTGL = PIN4_bm;
			}
			else if (def_ZB3_0)
			{
				PORTB.OUTSET = PIN4_bm;
			}
			
			if (!def_ZB4_git && !def_ZB4_0)
			{
				PORTB.OUTTGL = PIN5_bm;
			}
			else if (def_ZB4_0)
			{
				PORTB.OUTSET = PIN5_bm;
			}
			
			VAR_Flag_TCA_500ms = false;
		}
		
		if (VAR_Flag_TCA_200ms == true)
		{
			PORTB.OUTTGL = PIN6_bm;
			
			VAR_Flag_TCA_200ms = false;
		}
	}
	
// --------------------------------------------------------------------------------------------------------------


	if (def_ZB1_git && def_readcoil)	// ZB1_GIT
		PORTB.OUTCLR = PIN0_bm;
	else if (def_ZB1_0 && def_readcoil) // ZB1_0
		PORTB.OUTSET = PIN0_bm;

	if (def_ZB2_git && def_readcoil)	// ZB2_GIT
		PORTB.OUTCLR = PIN1_bm;
	else if (def_ZB2_0 && def_readcoil)	// ZB2_0
		PORTB.OUTSET = PIN1_bm;

	if (def_ZB3_nD_git && def_readcoil)	// ZB3_nD_GIT   swieci sie po przelaniu z zb1 i zb2, podczas wsypywania mruga
		PORTB.OUTCLR = PIN4_bm;

	if (def_ZB3_git && def_readcoil)	// ZB3_D_GIT	zaswieca po wsypaniu
		PORTB.OUTCLR = PIN4_bm;	
	else if (def_ZB3_0 && def_readcoil)	// ZB3_0
		PORTB.OUTSET = PIN4_bm;

	if (def_ZB4_git && def_readcoil)	// ZB4_GIT
		PORTB.OUTCLR = PIN5_bm;
	else if (def_ZB4_0 && def_readcoil)	// ZB4_0
		PORTB.OUTSET = PIN5_bm;


// --------------------------------------------------------------------------------------------------------------		
// ------------------------ MRUGANIE ZBIORNIKOW -----------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------

	while ((def_zawor_1 || def_zawor_2 || def_zawor_3 || def_zawor_4) && ((!def_ZB1_git && !def_ZB1_0) || (!def_ZB2_git && !def_ZB2_0)) && def_readcoil && !def_pause && !def_reset)	// zb1, zb2
	{
		if(VAR_Flag_PIT0)
		{
			USART0_sendFrame(0x01, 0x01, 0x8010, 0x0015, 0xC0D5);
			VAR_Flag_PIT0 = false;
		}
			
		if (VAR_Flag_TCA_500ms == true)
		{
			if (def_zawor_1 || def_zawor_3)
				PORTB.OUTTGL = PIN0_bm;
			else
				PORTB.OUTSET = PIN0_bm;
			
			if (def_zawor_2 || def_zawor_4)
				PORTB.OUTTGL = PIN1_bm;
			else
				PORTB.OUTSET = PIN1_bm;

			if (def_zawor_3 || def_zawor_4)
				PORTB.OUTTGL = PIN4_bm;
			
			if (def_ZB1_git && def_readcoil)	// ZB1_GIT
				PORTB.OUTCLR = PIN0_bm;
			
			if (def_ZB2_git && def_readcoil)	// ZB2_GIT
				PORTB.OUTCLR = PIN1_bm;
			
			VAR_Flag_TCA_500ms = false;
		}
		
		// ----------------------------------------------------------------
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
		// ---------------------------------------------------------------- 
	}
	
	while ((def_zawor_3 || def_zawor_4 || def_podajnik || def_mieszadlo || def_zawor_5) && !def_ZB3_git && !def_ZB3_0 && !def_ZB3_nD_git && def_readcoil && !def_pause && !def_reset)	// zb3
	{
		if(VAR_Flag_PIT0)
		{
			USART0_sendFrame(0x01, 0x01, 0x8010, 0x0015, 0xC0D5);
			VAR_Flag_PIT0 = false;
		}
		
		if (def_mieszadlo && def_readcoil)	// mieszadlo
			PORTA.OUTCLR = PIN7_bm;
		else
			PORTA.OUTSET = PIN7_bm;

		if (def_podajnik && def_readcoil)	// podajnik
			PORTC.OUTCLR = PIN3_bm;
		else
			PORTC.OUTSET = PIN3_bm;
		
		if (VAR_Flag_TCA_500ms == true)
		{
			if (def_zawor_3)
				PORTB.OUTTGL = PIN0_bm;
			else
				PORTB.OUTSET = PIN0_bm;
			
			if (def_zawor_4)
				PORTB.OUTTGL = PIN1_bm;
			else
				PORTB.OUTSET = PIN1_bm;
			
			PORTB.OUTTGL = PIN4_bm;
			
			if (def_zawor_5)
			{
				PORTB.OUTTGL = PIN5_bm;
			}
			
			VAR_Flag_TCA_500ms = false;
		}
		
		// ----------------------------------------------------------------
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
		// ----------------------------------------------------------------
	}
	
	while ((def_zawor_5 || def_zawor_6) && !def_ZB4_git && !def_ZB4_0 && def_readcoil && !def_pause && !def_reset)	// zb4
	{
		if(VAR_Flag_PIT0)
		{
			USART0_sendFrame(0x01, 0x01, 0x8010, 0x0015, 0xC0D5);
			VAR_Flag_PIT0 = false;
		}
				
		if (!def_zawor_5)
		{
			PORTB.OUTSET = PIN4_bm;
		}
		
		if (VAR_Flag_TCA_500ms == true)
		{
			if (def_zawor_5)
				PORTB.OUTTGL = PIN4_bm;
			else
				PORTB.OUTSET = PIN4_bm;
			
			PORTB.OUTTGL = PIN5_bm;
			
			VAR_Flag_TCA_500ms = false;
		}
		
		// ----------------------------------------------------------------
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
		// ----------------------------------------------------------------
	}
		
}

