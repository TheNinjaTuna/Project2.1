/*
 * AVR_ADC.c
 *
 * Created: 11/5/2019 3:38:07 PM
 * Author : Reinier, Ronald
 */ 

//includes nodig voor functies
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16E6 // used in _delay_ms, 16 MHz
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/sfr_defs.h>

//define compare and init
#define bit_is_set(sfr, bit) (_SFR_BYTE(sfr) & _BV(bit))
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define bit_is_set(sfr, bit) (_SFR_BYTE(sfr) & _BV(bit))
#define HIGH 0x1
#define LOW  0x0


//function declaration
void udrCheck();
void initPorts();
void read_adc();
void init();
void USARTInit(void);
unsigned char USARTReceive(void);
void USARTSend( unsigned char data);
void USARTPutstring(char *s);
void run();
void lichtport();
void setEcho();
int readTemp();
int readLight();
void fire_echo();

//variable declaration
uint16_t lichtValue;
uint16_t tempValue;
volatile uint32_t timervalue;

char stringtosend[16];

void init(){
	USARTInit();
	initPorts();
	
	ADMUX=(1<<REFS0)|(1<<ADLAR);
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
	FILE uart_output = FDEV_SETUP_STREAM(USARTSend, NULL, _FDEV_SETUP_WRITE);
	FILE uart_input = FDEV_SETUP_STREAM(NULL, USARTReceive, _FDEV_SETUP_READ);
	stdout = &uart_output;
	stdin = &uart_input;
		
	TCCR1A = 0;
	TCCR1B = 0;
			
	TIMSK1 |= (1 << TOIE1);
	TCNT1 = 0;
		
	EICRA = (1 << ISC10);
	EIMSK = (1 << INT1);
}

void initPorts(){
	//set ports as output
	DDRD = (1 << DDD2) | (1 << DDD3)| (1 << DDD4);
	//set ports on
	PORTD = (1 << PORTD2) | (1 << PORTD3) | (1 << PORTD4);
	
	DDRB = 0xff;
	PORTB = 0x00;
	DDRD = 0x16;
	PORTD = 0x00;
}

int main(void)
{
	init();
	sei();
	_delay_ms(500);
	run();
}

void run(){
	while (1)
	    {
		    read_adc();
			lichtport();
			USARTReceive();
			_delay_ms(30);
			udrCheck();
			
			setEcho(0);
			fire_echo();
			timervalue = (uint32_t)(timervalue*1024/16/58); //Berekening timer value
			
		    char str[100];
		    sprintf(str, "{\"sensorData\":{\"temp\":%d,\"light\":%d, \"ultrasonesensor\":%s}}",readTemp(),readLight(), itoa(timervalue, stringtosend, 10)); //32 + 3 + 3 = 38
		    USARTPutstring(str);
		    _delay_ms(500);
	    }
}