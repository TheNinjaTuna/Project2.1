/*
 * project21-arduino1.c
 *
 * Created: 22/10/2019 07:48:16
 * Author : reinier
 */ 

#define F_CPU 16E6
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define bit_is_set(sfr, bit) (_SFR_BYTE(sfr) & _BV(bit))
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)
#define HIGH 0x1
#define LOW  0x0
#define OUTPUT 0x1
#define INPUT  0x0

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <arduinoFunctions.c>

// Setting up pin layout
int redLed = 8; // Digital
int yellowLed = 9; // Digital
int greenLed = 10; // Digital
int Trig_pin = 11; // Digital
int Echo_pin = 12; // Digital
int lightSensor = 0; // Analog
int tempSensor = 1; // Analog

// Timers
uint32_t TTR_read_temp_sensor = 0;
uint32_t TTR_read_light_sensor = 0;
uint32_t TTR_read_distance_sensor = 0;
uint32_t TTR_transmit_readings = 0;
uint32_t TTR_turn_off_yellow = 0;
uint32_t TTR_handle_rolling = 0;

uint32_t timer_read_temp_sensor = 5;
uint32_t timer_read_light_sensor = 5;
uint32_t timer_read_distance_sensor = 5;
uint32_t timer_transmit_readings = 5;
uint32_t timer_turn_off_yellow = 5;
uint32_t timer_handle_rolling = 5;

uint32_t timer_enable_read_temp_sensor = 1;
uint32_t timer_enable_read_light_sensor = 1;
uint32_t timer_enable_read_distance_sensor = 1;
uint32_t timer_enable_transmit_readings = 1;
uint32_t timer_enable_turn_off_yellow = 0;
uint32_t timer_enable_handle_rolling = 1;

// Translation table
typedef enum {
	SEND_TEMP		= 0,
	SEND_LIGHT		= 1,
	CHANGE_MODE		= 2,
	SEND_MODE		= 3,
	SEND_STATE		= 4,
	ROLL_OUT		= 5,
	ROLL_IN			= 6,
	INC_TEMP		= 7,
	DEC_TEMP		= 8,
	INC_LIGHT		= 9,
	DEC_LIGHT		= 10,
	SEND_DISTANCE	= 11
} COMMANDS;

// Screen rolling
typedef enum {RIN=0, ROUT=1, ROLL=2} STATES;
STATES state = RIN;
typedef enum{AUTO=0, MANUAL=1} MODES;
MODES mode = AUTO;
uint8_t changeCount = 0;
uint8_t countTillChange = 2;
STATES nextState;

void initTimmer();
void initPorts();
void init();

int main(void)
{
	init();
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

void init(){
	initTimmer();
	initPorts();
	sei();
}


void initTimmer(){
	TCCR1B = (1 << CS12) | (1 << WGM12); // prescaler set to 256 (CTC mode)
	TIMSK1 = 1 << OCIE1A; // Timer 1 Output Compare A Match Interrupt Enable
	OCR1A = (uint16_t)625; // 10ms = (256/16.000.000)*625
}

void initPorts(){
	// LEDs (D)igital
	setPinMode('D', greenLed, OUTPUT);
	setPinMode('D', yellowLed, OUTPUT);
	setPinMode('D', redLed, OUTPUT);

	// Sensor pins (A)nalog
	setPinMode('A', lightSensor, INPUT);
	setPinMode('A', tempSensor, INPUT);
}

//this function shoud be call every 1 sec
ISR(TIMER1_OVF_vect){
	//invert de value of portb
	PORTB ^= PORTB;
}
