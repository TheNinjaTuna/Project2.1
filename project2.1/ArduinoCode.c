/*
 * ArduinoCode.c
 *
 * Created: 04/11/2019 07:28:54
 *  Author: reinier
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16E6 // used in _delay_ms, 16 MHz
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#define HIGH 0x1
#define LOW  0x0
#define OUTPUT 0x1
#define INPUT  0x0
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))


/*
PORTD maps to Arduino digital pins 0 to 7
PORTB maps to Arduino digital pins 8 to 13 The two high bits (6 & 7) map to the crystal pins and are not usable
PORTC maps to Arduino analog pins 0 to 5. Pins 6 & 7 are only accessible on the Arduino Mini
DDRC - The Port C Data Direction Register - read/write
PORTC - The Port C Data Register - read/write
PINC - The Port C Input Pins Register - read only
*/
// Pin functions #bron: gebaseerd op functie arduino C
void setPinMode(char type, int pin, uint8_t val) {
	switch(type) {
		case 'D':
		if(pin >= 8) {
			pin -= 8;
			if (val == LOW) { DDRB &= ~(_BV(pin)); }
			else { DDRB |= _BV(pin); }
			} else {
			if (val == LOW) { DDRD &= ~(_BV(pin)); }
			else { DDRD |= _BV(pin); }
		}
		break;
		case 'A':
		case 'C':
		if (val == LOW) { DDRC &= ~(_BV(pin)); }
		else { DDRC |= _BV(pin); }
		break;
	}
}
// #bron: gebaseerd op functie arduino C
void setPinVal(char type, int pin, uint8_t val) {
	switch(type) {
		case 'D':
		if(pin >= 8) {
			pin -= 8;
			if (val == LOW) { PORTB &= ~(_BV(pin)); }
			else { PORTB |= _BV(pin); }
			} else {
			if (val == LOW) { PORTD &= ~(_BV(pin)); }
			else { PORTD |= _BV(pin); }
		}
		break;
		case 'A':
		case 'C':
		if (val == LOW) {
			PORTC &= ~(_BV(pin)); // clear bit
			} else {
			PORTC |= _BV(pin); // set bit
		}
		break;
	}
}


int analogRead(uint8_t pin) {
	uint8_t low, high;
	#if defined(ADCSRB) && defined(MUX5)
	// the MUX5 bit of ADCSRB selects whether we're reading from channels
	// 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
	ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((pin >> 3) & 0x01) << MUX5);
	#endif
	ADMUX = (1 << 6) | (pin & 0x07);	// start the conversion
	sbi(ADCSRA, ADSC);
	// ADSC is cleared when the conversion finishes
	while (bit_is_set(ADCSRA, ADSC));
	// we have to read ADCL first; doing so locks both ADCL
	// and ADCH until ADCH is read.  reading ADCL second would
	// cause the results of each conversion to be discarded,
	// as ADCL and ADCH would be locked when it completed.
	low  = ADCL;
	high = ADCH;
	return (high << 8) | low;
}