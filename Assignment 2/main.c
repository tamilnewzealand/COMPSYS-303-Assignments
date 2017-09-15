/*
 * main.c
 *
 *  Created on: 9 செப்., 2017
 *      Author: சகாயன் சிற்சபேசன்
 */

#include "defines.h"
#include "sccharts.h"

alt_u32 avi_timer_isr(void* context)
{
	AVITO = 1;
}

alt_u32 aei_timer_isr(void* context)
{
	AEITO = 1;
}

alt_u32 pvarp_timer_isr(void* context)
{
	PVARPTO = 1;
}

alt_u32 vrp_timer_isr(void* context)
{
	VRPTO = 1;
}

alt_u32 lri_timer_isr(void* context)
{
	LRITO = 1;
}

alt_u32 uri_timer_isr(void* context)
{
	URITO = 1;
}

void init_buttons_pio()
{
	void* context = NULL;

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

void init_uart()
{
	void* context = NULL;

	// register the ISR
	alt_irq_register(UART_IRQ, context, uart_RecvBufferIsr);
}

void uart_RecvBufferIsr(void *context, alt_u32 id)
{
	recv_char = IORD_ALTERA_AVALON_UART_RXDATA(UART_BASE);
	if (recv_char == 'A') {
		ASense = 1;
		printf("A \n");
	}
	if (recv_char == 'V') {
		VSense = 1;
		printf("V \n");
	}
}

int main()
{
	mode = 0;
	init_buttons_pio();
	init_uart();
	reset();

	while(1)
	{
		tick();

		// Starts the timers if event occured
		if (AVIStart == 1) alt_alarm_start(&avi_timer, AVI_VALUE, avi_timer_isr, NULL);
		if (AEIStart == 1) alt_alarm_start(&aei_timer, AEI_VALUE, aei_timer_isr, NULL);
		if (PVARPStart == 1) alt_alarm_start(&pvarp_timer, PVARP_VALUE, pvarp_timer_isr, NULL);
		if (VRPStart == 1) alt_alarm_start(&vrp_timer, VRP_VALUE, vrp_timer_isr, NULL);
		if (LRIStart == 1) alt_alarm_start(&lri_timer, LRI_VALUE, lri_timer_isr, NULL);
		if (URIStart == 1) alt_alarm_start(&uri_timer, URI_VALUE, uri_timer_isr, NULL);
		
		// Stops the timers if event occured
		if (AVIStop == 1) alt_alarm_stop(&avi_timer);
		if (AEIStop == 1) alt_alarm_stop(&aei_timer);
		if (PVARPStop == 1) alt_alarm_stop(&pvarp_timer);
		if (VRPStop == 1) alt_alarm_stop(&vrp_timer);
		if (LRIStop == 1) alt_alarm_stop(&lri_timer);
		if (URIStop == 1) alt_alarm_stop(&uri_timer);

		// reseting inputs
		VSense = 0;
		ASense = 0;

		// resetting outputs
		AVITO = 0;
		PVARPTO = 0;
		VRPTO = 0;
		AEITO = 0;
		LRITO = 0;
		URITO = 0;

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

		// Outputting buffered values to LEDs
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

