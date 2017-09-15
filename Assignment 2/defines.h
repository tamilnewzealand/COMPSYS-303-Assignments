/*
 * defines.h
 *
 *  Created on: 10 செப்., 2017
 *      Author: சகாயன் சிற்சபேசன்
 */

#ifndef DEFINES_H_
#define DEFINES_H_

#include <stdio.h>
#include <system.h>
#include <sys/alt_alarm.h>
#include <altera_avalon_pio_regs.h>
#include <alt_types.h>
#include <sys/alt_irq.h>

// Example timeout values in milliseconds
#define AVI_VALUE 300
#define AEI_VALUE 800
#define PVARP_VALUE 50
#define VRP_VALUE 150
#define LRI_VALUE 950
#define URI_VALUE 900
#define LED_BUFFER 500

static volatile unsigned int oldKEY1;
static volatile unsigned int oldKEY2;

static volatile int recv_char;

static unsigned int LEDVPace;
static unsigned int LEDAPace;
static unsigned int LEDBufferCountA;
static unsigned int LEDBufferCountV;

alt_u32 avi_timer_isr(void* context);
alt_u32 aei_timer_isr(void* context);
alt_u32 pvarp_timer_isr(void* context);
alt_u32 vrp_timer_isr(void* context);
alt_u32 lri_timer_isr(void* context);
alt_u32 uri_timer_isr(void* context);

static alt_alarm timer_AVI;
static alt_alarm timer_AEI;
static alt_alarm timer_PVARP;
static alt_alarm timer_VRP;
static alt_alarm timer_LRI;
static alt_alarm timer_URI;

void init_buttons_pio();
void buttons_isr(void* context, alt_u32 id);
void init_uart();
void uart_RecvBufferIsr(void *context, alt_u32 id);
int main();

#endif /* DEFINES_H_ */
