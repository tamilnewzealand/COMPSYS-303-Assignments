/*
 * defines.h
 *
 *  Created on: 10 செப்., 2017
 *      Author: சகாயன் சிற்சபேசன்
 */

#ifndef DEFINES_H_
#define DEFINES_H_

#include <stdio.h>

// Example timeout values in milliseconds
#define AVI_VALUE 300
#define AEI_VALUE 800
#define PVARP_VALUE 50
#define VRP_VALUE 150
#define LRI_VALUE 950
#define URI_VALUE 900
#define LED_BUFFER 500

static unsigned int mode;
static volatile unsigned int oldKEY1;
static volatile unsigned int oldKEY2;

static unsigned int LEDVPace;
static unsigned int LEDAPace;
static unsigned int LEDBufferCountA;
static unsigned int LEDBufferCountV;

void init_buttons_pio();
void buttons_isr(void* context, alt_u32 id);
int main();

#endif /* DEFINES_H_ */
