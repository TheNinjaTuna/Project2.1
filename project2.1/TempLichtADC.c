//includes nodig voor functies
#include <avr/io.h>
#define F_CPU 16E6 // used in _delay_ms, 16 MHz

//function declaration
void udrCheck();
void initPorts();
void read_adc();
void lichtport();

//variable declaration
uint16_t lichtValue;
uint16_t tempValue;

int readTemp(){
	float voltage = tempValue * 5.0 / 1024;
	return (voltage - 0.5) * 100;
}

int readLight(){
	return lichtValue + 0.0;
}

void lichtport(){
	if(lichtValue < 490){
		PORTD = (1 << PORTD2) | (1 << PORTD3) | (1 << PORTD4) ;
		}else{
		PORTD = (0 << PORTD2) | (0 << PORTD3) | (0 << PORTD4) ;
	}
}

void udrCheck(){
	if(UDR0 != 0x00){
		read_adc();
	}
}

void read_adc(){
	uint8_t theLow = ADCL;
	//uint16_t  theTenBitResult = ADCH<<8 | theLow;
	uint16_t  theTenBitResult = ADCH<<2 | theLow >> 6;
	
	if(ADMUX == 0x60){
		lichtValue = theTenBitResult;
		ADMUX = 0x61;
		}else if(ADMUX == 0x61){
		tempValue = theTenBitResult;
		ADMUX = 0x60;
		}else{
		ADMUX = 0x60;
	}
	
	ADCSRA |= (1<<ADSC);
}