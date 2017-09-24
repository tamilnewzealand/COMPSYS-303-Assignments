/*
* main.c
*
*  Created on: 9 செப்., 2017
*      Author: சகாயன் சிற்சபேசன்
*/

#include "defines.h"
#include "sccharts.h"

/*
 * Following are Timer ISRs for the various timers
 * in a DDD mode Pacemaker. Each ISR is triggered
 * when the predefined timeout value has been reached.
 * The associate Timeout variable is set and the
 * associated signal buffer timer is started. A return
 * value of 0 is used to prevent the timer from
 * automatically restarting.
 */

alt_u32 avi_timer_isr(void* context)
{
	AVITO = 1;
	alt_alarm_start(&timer_AVITO, SIGNAL_BUFFER, avito_timer_isr, NULL);
	printf("\nAVI Timed out");
	return 0;
}

alt_u32 aei_timer_isr(void* context)
{
	AEITO = 1;
	alt_alarm_start(&timer_AEITO, SIGNAL_BUFFER, aeito_timer_isr, NULL);
	printf("\nAEI Timed out");
	return 0;
}

alt_u32 pvarp_timer_isr(void* context)
{
	PVARPTO = 1;
	alt_alarm_start(&timer_PVARPTO, SIGNAL_BUFFER, pvarpto_timer_isr, NULL);
	printf("\nPVARP Timed out");
	return 0;
}

alt_u32 vrp_timer_isr(void* context)
{
	VRPTO = 1;
	alt_alarm_start(&timer_VRPTO, SIGNAL_BUFFER, vrpto_timer_isr, NULL);
	printf("\nVRP Timed out");
	return 0;
}

alt_u32 lri_timer_isr(void* context)
{
	LRITO = 1;
	alt_alarm_start(&timer_LRITO, SIGNAL_BUFFER, lrito_timer_isr, NULL);
	printf("\nLRI Timed out");
	return 0;
}

alt_u32 uri_timer_isr(void* context)
{
	URITO = 1;
	alt_alarm_start(&timer_URITO, SIGNAL_BUFFER, urito_timer_isr, NULL);
	printf("\nURI Timed out");
	return 0;
}

/*
 * Signal buffering timers.
 * These timeout after predefined values and reset
 * the variables back to 0. This is to prevent
 * variables from staying high for too long.
 */
alt_u32 leda_timer_isr(void* context)
{
	LEDAPace = 0;
	return 0;
}

alt_u32 ledv_timer_isr(void* context)
{
	LEDVPace = 0;
	return 0;
}

alt_u32 asense_timer_isr(void* context)
{
	ASense = 0;
	return 0;
}

alt_u32 vsense_timer_isr(void* context)
{
	VSense = 0;
	return 0;
}

alt_u32 avito_timer_isr(void* context)
{
	AVITO = 0;
	return 0;
}

alt_u32 aeito_timer_isr(void* context)
{
	AEITO = 0;
	return 0;
}

alt_u32 pvarpto_timer_isr(void* context)
{
	PVARPTO = 0;
	return 0;
}

alt_u32 vrpto_timer_isr(void* context)
{
	VRPTO = 0;
	return 0;
}

alt_u32 lrito_timer_isr(void* context)
{
	LRITO = 0;
	return 0;
}

alt_u32 urito_timer_isr(void* context)
{
	URITO = 0;
	return 0;
}

void init_buttons_pio()
{
	// Don't need a context as dealing with global variable only.
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

	// Key debouncing: variable only set on rising edge
	// Signal buffer timer used to ensure variable is reset
	// after appropriate timings.
	if (((*temp) & 0x02) > 0)
	{
		 if (oldKEY1 == 0) {
			 ASense = 1;
			 alt_alarm_start(&timer_asense, SIGNAL_BUFFER, asense_timer_isr, NULL);
		 }
		 oldKEY1 = 1;
	}
	else oldKEY1 = 0;

	if (((*temp) & 0x01) > 0)
	{
		 if (oldKEY2 == 0) {
			 VSense = 1;
			 alt_alarm_start(&timer_vsense, SIGNAL_BUFFER, vsense_timer_isr, NULL);
		 }
		 oldKEY2 = 1;
	}
	else oldKEY2 = 0;
}

void read_uart()
{
	// Trys reading a single character from UART
	if ((nbr = read(up, buffer, 1)) > 0) {
		if (buffer[0] == 65) {
			ASense = 1; // A
			alt_alarm_start(&timer_asense, SIGNAL_BUFFER, asense_timer_isr, NULL);
		}
		if (buffer[0] == 86) {
			VSense = 1; // V
			alt_alarm_start(&timer_vsense, SIGNAL_BUFFER, vsense_timer_isr, NULL);
		}
	}
}

/*
 * Starts and stops timers based on inputs from
 * SCCharts generated code.
 */
void start_stop_timers()
{
	// Starts the timers if event occurred
	if (AVI_start > 0) {
		AVITO = 0;
		alt_alarm_stop(&timer_AVI);
		alt_alarm_start(&timer_AVI, AVI_VALUE, avi_timer_isr, NULL);
		printf("\nAVI Start");
	}
	if (AEI_start > 0) {
		AEITO = 0;
		alt_alarm_stop(&timer_AEI);
		alt_alarm_start(&timer_AEI, AEI_VALUE, aei_timer_isr, NULL);
		printf("\nAEI Start");
	}
	if (PVARP_start > 0) {
		PVARPTO = 0;
		alt_alarm_stop(&timer_PVARP);
		alt_alarm_start(&timer_PVARP, PVARP_VALUE, pvarp_timer_isr, NULL);
		printf("\nPVARP Start");
	}
	if (VRP_start > 0) {
		VRPTO = 0;
		alt_alarm_start(&timer_VRP, VRP_VALUE, vrp_timer_isr, NULL);
		printf("\nVRP Start");
	}
	if (LRI_start > 0) {
		LRITO = 0;
		alt_alarm_start(&timer_LRI, LRI_VALUE, lri_timer_isr, NULL);
		printf("\nLRI Start");
	}
	if (URI_start > 0) {
		URITO = 0;
		alt_alarm_stop(&timer_URI);
		alt_alarm_start(&timer_URI, URI_VALUE, uri_timer_isr, NULL);
		printf("\nURI Start");
	}

	// Stops the timers if event occurred
	if (AVI_stop > 0) {
		alt_alarm_stop(&timer_AVI);
		AVITO = 0;
		printf("\nAVI Stopped");
	}
	if (AEI_stop > 0) {
		alt_alarm_stop(&timer_AEI);
		AEITO = 0;
		printf("\nAEI Stopped");
	}
	if (LRI_stop > 0) {
		alt_alarm_stop(&timer_LRI);
		LRITO = 0;
		printf("\nLRI Stopped");
	}
}

void show_leds()
{
	// Buffering outputs
	if (APace > 0)
	{
		fprintf(fp, "\nA");
		printf("\nApace");
		LEDAPace = 1;
		alt_alarm_stop(&timer_leda);
		alt_alarm_start(&timer_leda, LED_BUFFER, leda_timer_isr, NULL);
	}
	if (VPace > 0)
	{
		fprintf(fp, "\nV");
		printf("\nVpace");
		LEDVPace = 1;
		alt_alarm_stop(&timer_ledv);
		alt_alarm_start(&timer_ledv, LED_BUFFER, ledv_timer_isr, NULL);
	}

	// Outputing buffered values to the LEDs
	if (LEDVPace > 0)
	{
		if (LEDAPace > 0) IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, 0x03);
		else IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, 0x01);
	}
	else
	{
		if (LEDAPace > 0) IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, 0x02);
		else IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, 0x00);
	}
}

int main()
{
	init_buttons_pio();
	up = open(UART_NAME, O_RDWR | O_NONBLOCK);
	fp = fopen(UART_NAME, "w+");
	reset();

	while(1)
	{
		read_uart();
		tick();
		start_stop_timers();
		show_leds();
	}

	close(up);
	fclose(fp);
	return 0;
}

