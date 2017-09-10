/*
 * main.c
 *
 *  Created on: 9 செப்., 2017
 *      Author: சகாயன் சிற்சபேசன்
 */

#include "defines.h"
#include "sccharts.h"

void init_buttons_pio()
{
	void* context = (void*) &buttonValue;

	// clears the edge capture register
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTONS_BASE, 0);
	// enable interrupts for all buttons
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BUTTONS_BASE, 0x7);

	// register the ISR
	alt_irq_register(BUTTONS_IRQ, context, buttons_isr);
}

void buttons_isr(void* context, alt_u32 id)
{
	int* temp = (int*) context; // need to cast the context first before using it
	(*temp) = IORD_ALTERA_AVALON_PIO_EDGE_CAP(BUTTONS_BASE);
	// clear the edge capture register
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTONS_BASE, 0);

	if (((*temp) & 0x02) > 0)
	{
		if (oldKEY1 == 0) ASense = 1;
		oldKEY1 = 1;
	}
	else oldKEY1 = 0;
	
	if (((*temp) & 0x01) > 0)
	{
		if (oldKEY2 == 0) VSense = 1;
		oldKEY2 = 1;
	}
	else oldKEY2 = 0;
}

int main()
{
	mode = 0;
	reset();

	while(1)
	{
		tick();

		// reseting inputs
		VSense = 0;
		ASense = 0;

		// Buffering outputs
		if (APace == 1)
		{
			LEDAPace = APace;
			LEDBufferCountA = LED_BUFFER;
		}

		if (VPace == 1)
		{
			LEDVPace = VPace;
			LEDBufferCountV = LED_BUFFER;
		}

		LEDBufferCountA--;
		LEDBufferCountV--;

		if (LEDBufferCountA == 0) LEDAPace = 0;
		if (LEDBufferCountV == 0) LEDVPace = 0;

		// Outputing buffered values to LEDs
		if (LEDVPace == 1)
		{
			if (LEDAPace == 1) IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, 0x03);
			else IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, 0x01);
		}
		else
		{
			if (LEDAPace == 1) IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, 0x02);
			else IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, 0x00);
		}
	
	}
	return 0;
}