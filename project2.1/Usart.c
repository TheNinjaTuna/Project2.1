/*
 * usart.c
 *
 * Created: 04/11/2019 07:20:58
 *  Author: Reinier, Ronald
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <avr/sfr_defs.h>
#define F_CPU 16E6 // used in _delay_ms, 16 MHz
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

void USARTInit(void) {
	UBRR0H = (uint8_t)((int)BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)((int)BAUD_PRESCALLER);
	UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
	UCSR0B |= _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */	
}

unsigned char USARTReceive(FILE *stream) {
	while (!(UCSR0A & _BV(RXC0)));
	return UDR0;
}

void USARTSend(unsigned char data, FILE *stream) {
	while (!(UCSR0A & _BV(UDRE0)));
	UDR0 = data;
}

void USARTPutstring(char *s, FILE *stream) {
	while (*s != 0x00) {
		USARTSend(*s, stream);
		s++;
	}
	USARTSend('\n', stream);
}