/* Traffic Light Controller
*
* --- Code is best viewed with the tab size of 4. ---
*/

#include <system.h>
#include <sys/alt_alarm.h>
#include <sys/alt_irq.h>
#include <altera_avalon_pio_regs.h>
#include <alt_types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// A template for COMPSYS 303 Assignment 1
//
// NOTE: You do not need to use this! Feel free
// to write your own code from scratch if you
// want, this is purely an example

// FUNCTION PROTOTYPES
// Timer ISRs
alt_u32 tlc_timer_isr(void* context);
alt_u32 camera_timer_isr(void* context);
alt_u32 timer_isr_function(void* context);

//  Misc
// Others maybe added eg LEDs / UART
void lcd_set_mode(unsigned int mode);

// TLC state machine functions
void init_tlc(void);
void simple_tlc(int* state);
void pedestrian_tlc(int* state);
void configurable_tlc(int* state);
void camera_tlc(int* state);

// Button Inputs / Interrupts
void handle_mode_button();
void handle_vehicle_button(void);
void init_buttons_pio(void);
void NSEW_ped_isr(void* context, alt_u32 id);

// Configuration Functions
void timeout_data_handler(void);


// CONSTANTS
#define OPERATION_MODES 0x03	// number of operation modes (00 - 03 = 4 modes)
#define CAMERA_TIMEOUT	2000	// timeout period of red light camera (in ms)
#define TIMEOUT_NUM 6			// number of timeouts
#define TIME_LEN 8				// buffer length for time digits

#define ESC 27
#define CLEAR_LCD_STRING "[2J"


// USER DATA TYPES
// Timeout buffer structure
typedef struct  {
	int index;
	unsigned int timeout[TIMEOUT_NUM];
} TimeBuf;


// GLOBAL VARIABLES
static alt_alarm tlc_timer;		// alarm used for traffic light timing
static alt_alarm camera_timer;	// alarm used for camera timing
static alt_alarm timer;	// alarm used for camera timing

// NOTE:
// set contexts for ISRs to be volatile to avoid unwanted Compiler optimisation
static volatile int tlc_timer_event = 0;
static volatile int camera_timer_event = 0;
static volatile int pedestrianNS = 0;
static volatile int pedestrianEW = 0;
static volatile int pedestrianNSstate = 0;
static volatile int pedestrianESstate = 0;
static volatile int pedestrianState = 0;
static volatile int button_count = 0;
static volatile int carEnter = 0;
static volatile int carExit = 0;
static volatile int red_light_flag = 0;
static volatile int red_light_clear = 0;
static volatile int timerCount = 0;
static volatile int timeCountMain = 0;
static volatile int buttonValue = 1;
static volatile int NSWEFlag = 0;


// Traffic light timeouts
static unsigned int timeout[TIMEOUT_NUM] = {6000, 500, 2000, 6000, 500, 2000};
static TimeBuf timeout_buf = { -1, {500, 6000, 2000, 500, 6000, 2000} };

// UART
FILE* up;
FILE* lcd;

// Traffic light LED values
//static unsigned char traffic_lights[TIMEOUT_NUM] = {0x90, 0x50, 0x30, 0x90, 0x88, 0x84};
// NS RGY | EW RGY
// NR,NG | NY,ER,EG,EY
static unsigned char traffic_lights[TIMEOUT_NUM] = {0x24, 0x14, 0x0C, 0x24, 0x22, 0x21};

enum traffic_states {RR0, GR, YR, RR1, RG, RY};

static unsigned int mode = 0;
static unsigned int newMode = 0;
// Process states: use -1 as initialization state
static int proc_state[OPERATION_MODES + 1] = {-1, -1, -1, -1};


// Initialize the traffic light controller
// for any / all modes
void init_tlc(void)
{
	pedestrianESstate = 0;
	pedestrianNSstate = 0;
	pedestrianState = 0;
	pedestrianEW = 0;
	pedestrianNS = 0;
}


/* DESCRIPTION: Writes the mode to the LCD screen
* PARAMETER:   mode - the current mode
* RETURNS:     none
*/
void lcd_set_mode(unsigned int mode)
{
	if(lcd != NULL) {
		fprintf(lcd, "%c%s", ESC, CLEAR_LCD_STRING);
		fprintf(lcd, "Current Mode: %d\n", mode);
	}

}

/* DESCRIPTION: Updates the ID of the task to be executed and the 7-segment display
* PARAMETER:   taskid - current task ID
* RETURNS:     none
*/
void handle_mode_button()
{
	newMode = IORD_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE) & 0x03;
	//printf("Current Mode: %d \n", newMode);
	//Check if mode was changed
	if (newMode != mode) {
		//Change mode only if in a safe state (i.e. in a R-R situation) and re-initialize them
		if ((proc_state[mode] == RR0) | (proc_state[mode] == RR1)) {
			proc_state[mode] = -1;
			mode = newMode;
			proc_state[mode] = -1;
			lcd_set_mode(mode);
			alt_alarm_stop(&tlc_timer);
			alt_alarm_stop(&camera_timer);
		}
	}
}


/* DESCRIPTION: Simple traffic light controller
* PARAMETER:   state - state of the controller
* RETURNS:     none
*/
void simple_tlc(int* state)
{
	if (*state == -1) {
		// Process initialization state
		init_tlc();
		(*state)++;
		void* timerContext = (void*) &timerCount;
		alt_alarm_start(&tlc_timer, timeout[(*state)], tlc_timer_isr, timerContext);
		return;
	}

	if (timerCount > 0) {
		void* timerContext = (void*) &timerCount;
		alt_alarm_start(&tlc_timer, timeout[(*state)], tlc_timer_isr, timerContext);

		// Increase state number (within bounds)
		(*state)++;
		if (*state == 6) *state = 0;

		timerCount = 0;
	}

	//Switch which LED is on based on mode
	switch (*state) {
		case RR0:
			//LED bit 5 and 2 on (0010 0100)
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, traffic_lights[0]);//0x24
			break;
		case YR:
			//LED bit 5 and 0 on (0001 0100)
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, traffic_lights[1]);//0x14
			break;
		case GR:
			//LED bit 5 and 1 on (0000 1100)
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, traffic_lights[2]);//0x0C
			break;
		case RR1:
			//LED bit 5 and 2 on (0010 0100)
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, traffic_lights[3]);//0x24
			break;
		case RY:
			//LED bit 3 and 2 on (0010 0010)
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, traffic_lights[4]);//0x22
			break;
		case RG:
			//LED bit 4 and 2 on (0010 0001)
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, traffic_lights[5]);//0x21
			break;
		default:
			//All LED off
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, 0xFF);
			break;
	}
}


/* DESCRIPTION: Handles the traffic light timer interrupt
* PARAMETER:   context - opaque reference to user data
* RETURNS:     Number of 'ticks' until the next timer interrupt. A return value
*              of zero stops the timer.
*/
alt_u32 tlc_timer_isr(void* context)
{
	volatile int* trigger = (volatile int*)context;
	*trigger = 1;
	return 0;
}


/* DESCRIPTION: Initialize the interrupts for all buttons
* PARAMETER:   none
* RETURNS:     none
*/
void init_buttons_pio(void)
{
	void* context = (void*) &buttonValue;

	// clears the edge capture register
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTONS_BASE, 0);
	// enable interrupts for all buttons
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BUTTONS_BASE, 0x7);

	// register the ISR
	alt_irq_register(BUTTONS_IRQ, context, NSEW_ped_isr);
}


/* DESCRIPTION: Pedestrian traffic light controller
* PARAMETER:   state - state of the controller
* RETURNS:     none
*/
void pedestrian_tlc(int* state)
{
	if (*state == -1) {
		// Process initialization state
		init_tlc();
		(*state)++;
		void* timerContext = (void*) &timerCount;
		alt_alarm_start(&tlc_timer, timeout[(*state)], tlc_timer_isr, timerContext);
		return;
	}

	if (timerCount > 0) {
		void* timerContext = (void*) &timerCount;
		alt_alarm_start(&tlc_timer, timeout[(*state)], tlc_timer_isr, timerContext);

		// Increase state number (within bounds)
		(*state)++;
		if (*state == 6) *state = 0;

		if (((*state) == 0) && (pedestrianNS == 1)) pedestrianState = 1;

		if (*state == 2) {
			if (pedestrianState == 1) pedestrianNS = 0;
			pedestrianState = 0;		
		}

		if (((*state) == 3) && (pedestrianEW == 1)) pedestrianState = 1;

		if (*state == 5) {
			if (pedestrianState == 1) pedestrianEW = 0;
			pedestrianState = 0;
		}

		timerCount = 0;
	}

	//Switch which LED is on based on mode
	switch (*state) {
		case RR0:
			//LED bit 5 and 2 on (0010 0100)
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, traffic_lights[0]);//0x24
			break;
		case YR:
			//LED bit 5 and 0 on (0001 0100)
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, traffic_lights[1]);//0x14
			break;
		case GR:
			//LED bit 5 and 1 on (0000 1100)
			if (pedestrianState == 0) IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, traffic_lights[2]);//0x0C
			else IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, 0x8C);//0x8C
			break;
		case RR1:
			//LED bit 5 and 2 on (0010 0100)
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, traffic_lights[3]);//0x24
			break;
		case RY:
			//LED bit 3 and 2 on (0010 0010)
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, traffic_lights[4]);//0x22
			break;
		case RG:
			//LED bit 4 and 2 on (0010 0001)
			if (pedestrianState == 0) IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, traffic_lights[5]);//0x21
			else IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, 0x61);//0x61
			break;
		default:
			//All LED off
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, 0xFF);
			break;
	}
}


/* DESCRIPTION: Handles the NSEW pedestrian button interrupt
* PARAMETER:   context - opaque reference to user data
*              id - hardware interrupt number for the device
* RETURNS:     none
*/
void NSEW_ped_isr(void* context, alt_u32 id)
{
	// NOTE:
	// Cast context to volatile to avoid unwanted compiler optimization.
	// Store the value in the Button's edge capture register in *context

	int* temp = (int*) context; // need to cast the context first before using it
	(*temp) = IORD_ALTERA_AVALON_PIO_EDGE_CAP(BUTTONS_BASE);
	// clear the edge capture register
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTONS_BASE, 0);

	if (((*temp) & 0x02) > 0) pedestrianNS = 1;
	if (((*temp) & 0x01) > 0) pedestrianEW = 1;

	if (((*temp) & 0x04) > 0) {
		if (NSWEFlag == 0) {
			if (carEnter == 0) {
				carEnter = 1;
				carExit = 0;
			} else {
				carExit = 1;
			}
		}
		NSWEFlag = 1;
	} else {
		NSWEFlag = 0;
	}

	
}


/* DESCRIPTION: Configurable traffic light controller
* PARAMETER:   state - state of the controller
* RETURNS:     none
*/
/*
If there is new configuration data... Load it.
Else run pedestrian_tlc();
*/
void configurable_tlc(int* state)
{
	//Only reconfigure when in a safe state (i.e. R-R) and if the switch is used
	if (((proc_state[mode] == RR0) || (proc_state[mode] == RR1)) && ((IORD_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE) & 0x04) > 0)) timeout_data_handler();
	else pedestrian_tlc(state);
}


/* DESCRIPTION: Parses the configuration string and updates the timeouts
* PARAMETER:   none
* RETURNS:     none
*/
void timeout_data_handler(void)
{
	int payload[32];
	int buffered_values[6];
	int i = 0;
	int j = 5;

	if (up != NULL)
	{
		//store the data into payload and only store numbers or commas (ASCII values 44 and 48 to 57)
		while (payload[i-1] != 10)
		{
			payload[i] = fgetc(up);
			if (payload[i] > 57) continue;
			if ((payload[i] < 44) && (payload[i] != 10)) continue;
			if (payload[i] == 45) continue;
			if (payload[i] == 46) continue;
			if (payload[i] == 47) continue;
			printf("%u", payload[i]);
			i++;
		}
		i = i - 2;

		while (i >= 0)
		{
			if (payload[i] == 10) i--;
			if (payload[i] != 44)
			{
				//Store the unit
				buffered_values[j] = (payload[i] - 48);
				i--;

				if (payload[i] != 44)
				{
					//Store the tens (if any)
					buffered_values[j] = buffered_values[j] + ((payload[i] - 48) * 10);
					i--;

					if (payload[i] != 44)
					{
						//Store the hundreds (if any)
						buffered_values[j] = buffered_values[j] + ((payload[i] - 48) * 100);
						i--;

						if (payload[i] != 44)
						{
							//Store the thousands (if any)
							buffered_values[j] = buffered_values[j] + ((payload[i] - 48) * 1000);
							i--;
						}
					}
				}
				j--;
			}
			else
			{
				i--;
			}
		}
		printf("%d", buffered_values[0]);
	}

	for (i = 0; i < 6; i++) {
		timeout[i] = buffered_values[i];
	}

}

/* DESCRIPTION: Handles the red light camera timer interrupt
* PARAMETER:   context - opaque reference to user data
* RETURNS:     Number of 'ticks' until the next timer interrupt. A return value
*              of zero stops the timer.
*/
alt_u32 camera_timer_isr(void* context)
{
	volatile int* trigger = (volatile int*)context;
	*trigger = 1;
	return 0;
}

alt_u32 timer_isr_function(void* context)
{
	int *timeCount = (int*) context;
	(*timeCount)++;
	return 1000;
}

/* DESCRIPTION: Camera traffic light controller
* PARAMETER:   state - state of the controller
* RETURNS:     none
*/
/*
Same functionality as configurable_tlc
But also handles Red-light camera
*/
void camera_tlc(int* state)
{
	configurable_tlc(state);
	if (((*state) == YR) || ((*state) == RY)) {
		if (carEnter == 1) {
			fprintf(up, "Camera Activated\n");
			red_light_flag = 0;
			void* timerContext = (void*) &red_light_flag;
			alt_alarm_start(&camera_timer, CAMERA_TIMEOUT, camera_timer_isr, timerContext);

			timeCountMain = 0;
			void* timerContextMain = (void*) &timeCountMain;
			alt_alarm_start(&timer, 1000, timer_isr_function, timerContextMain);
		}
	}

	if (carEnter == 1) {
		if (carExit == 1) {
			carEnter = 0;
			if (red_light_flag == 1) {
				fprintf(up, "Snapshot Taken\n");
				fprintf(up, "Time in Intersection: %d sec \n", timeCountMain);
				alt_alarm_stop(&timer);
			}
			else
			{
				fprintf(up, "Vehicle Left\n");
				fprintf(up, "Time in Intersection: %d sec \n", timeCountMain);
				alt_alarm_stop(&timer);
			}
		}
	}
}

int main(void)
{
	int buttons = 0;			// status of mode button

	lcd = fopen(LCD_NAME, "w");
	up = fopen(UART_NAME, "w+");

	lcd_set_mode(0);			// initialize lcd
	init_buttons_pio();			// initialize buttons

	while (1) {

		handle_mode_button();

		// Execute the correct TLC
		switch (mode) {
			//Mode 1
			case 0:
				simple_tlc(&proc_state[0]);
				break;
			//Mode 2
			case 1:
				pedestrian_tlc(&proc_state[1]);
				break;
			//Mode 3
			case 2:
				configurable_tlc(&proc_state[2]);
				break;
			//Mode 4
			case 3:
				camera_tlc(&proc_state[3]);
				break;
		}
	}

	fclose(up);
	fclose(lcd);
	return 1;
}
