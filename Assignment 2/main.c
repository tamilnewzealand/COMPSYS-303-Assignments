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
 	printf("Hello from Nios II!\n");

	return 0;
}
