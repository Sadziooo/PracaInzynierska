#include "Debouncer.h"

void debounce0(unsigned int x, unsigned int y, unsigned int z)
{
	z=PORT_PULLUPEN_bm;
	//Counter for number of equal states
	static uint8_t count = 0;
	//Keeps track of current (debounced) state
	static uint8_t button_state = 0;
	//Check if button is high or low for the moment
	uint8_t current_state = (~x & y);
	if (current_state != button_state) 
	{
		//Button state is about to be changed, increase counter
		count++;
		if (count >= 4) 
		{
			//The button have not bounced for four checks, change state
			button_state = current_state;
			//If the button was pressed (not released), tell main so
			if (current_state != 0) 
			{
				button0_down = 1;
			}
			count = 0;
		}
	} 
	else 
	{
		//Reset counter
		count = 0;
	}
}

void debounce1(unsigned int x, unsigned int y, unsigned int z)
{
	z=PORT_PULLUPEN_bm;
	//Counter for number of equal states
	static uint8_t count = 0;
	//Keeps track of current (debounced) state
	static uint8_t button_state = 0;
	//Check if button is high or low for the moment
	uint8_t current_state = (~x & y);
	if (current_state != button_state)
	{
		//Button state is about to be changed, increase counter
		count++;
		if (count >= 4)
		{
			//The button have not bounced for four checks, change state
			button_state = current_state;
			//If the button was pressed (not released), tell main so
			if (current_state != 0)
			{
				button1_down = 1;
			}
			count = 0;
		}
	}
	else
	{
		//Reset counter
		count = 0;
	}
}

void debounce2(unsigned int x, unsigned int y, unsigned int z)
{
	z=PORT_PULLUPEN_bm;
	//Counter for number of equal states
	static uint8_t count = 0;
	//Keeps track of current (debounced) state
	static uint8_t button_state = 0;
	//Check if button is high or low for the moment
	uint8_t current_state = (~x & y);
	if (current_state != button_state)
	{
		//Button state is about to be changed, increase counter
		count++;
		if (count >= 4)
		{
			//The button have not bounced for four checks, change state
			button_state = current_state;
			//If the button was pressed (not released), tell main so
			if (current_state != 0)
			{
				button2_down = 1;
			}
			count = 0;
		}
	}
	else
	{
		//Reset counter
		count = 0;
	}
}
