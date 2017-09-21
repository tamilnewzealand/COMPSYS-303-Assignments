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
	 return 0;
 }
 
 alt_u32 aei_timer_isr(void* context)
 {
	 AEITO = 1;
	 return 0;
 }
 
 alt_u32 pvarp_timer_isr(void* context)
 {
	 PVARPTO = 1;
	 return 0;
 }
 
 alt_u32 vrp_timer_isr(void* context)
 {
	 VRPTO = 1;
	 return 0;
 }
 
 alt_u32 lri_timer_isr(void* context)
 {
	 LRITO = 1;
	 printf("\nLRI Timed out");
	 return 0;
 }
 
 alt_u32 uri_timer_isr(void* context)
 {
	 URITO = 1;
	 return 0;
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
 
 void read_uart()
 {
	 if ((nbr = read(up, buffer, 1)) > 0) {
		 if (buffer[0] == 65) ASense = 1; // A
		 if (buffer[0] == 86) VSense = 1; // V
	 }
 }
 
 void start_stop_timers()
 {
	 // Starts the timers if event occurred
	 if (AVI_start == 1) alt_alarm_start(&timer_AVI, AVI_VALUE, avi_timer_isr, NULL);
	 if (AEI_start == 1) alt_alarm_start(&timer_AEI, AEI_VALUE, aei_timer_isr, NULL);
	 if (PVARP_start == 1) alt_alarm_start(&timer_PVARP, PVARP_VALUE, pvarp_timer_isr, NULL);
	 if (VRP_start == 1) alt_alarm_start(&timer_VRP, VRP_VALUE, vrp_timer_isr, NULL);
	 if (LRI_start == 1) alt_alarm_start(&timer_LRI, LRI_VALUE, lri_timer_isr, NULL);
	 if (URI_start == 1) alt_alarm_start(&timer_URI, URI_VALUE, uri_timer_isr, NULL);
 
	 // Stops the timers if event occurred
	 if (AVI_stop == 1) alt_alarm_stop(&timer_AVI);
	 if (AEI_stop == 1) alt_alarm_stop(&timer_AEI);
	 if (PVARP_stop == 1) alt_alarm_stop(&timer_PVARP);
	 if (VRP_stop == 1) alt_alarm_stop(&timer_VRP);
	 if (LRI_stop == 1) alt_alarm_stop(&timer_LRI);
	 if (URI_stop == 1) alt_alarm_stop(&timer_URI);
 }
 
 void reset_inputs()
 {
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
 }
 
 void show_leds()
 {
	 // Buffering outputs
	 if (APace == 1)
	 {
		 fprintf(fp, "A");
		 LEDAPace = APace;
		 LEDBufferCountA = LED_BUFFER;
	 }
 
	 if (VPace == 1)
	 {
		 fprintf(fp, "V");
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
 
 int main()
 {
	 init_buttons_pio();
	 up = open(UART_NAME, O_RDWR | O_NONBLOCK);
	 fp = fopen(UART_NAME, "w+");
	 reset();
	 VSense = 1;
	 LEDBufferCountA = 0;
	 LEDBufferCountV = 0;
 
	 while(1)
	 {
		 read_uart();
		 tick();
		 start_stop_timers();
		 reset_inputs();
		 show_leds();
	 }
 
	 close(up);
	 fclose(fp);
	 return 0;
 }
 
 