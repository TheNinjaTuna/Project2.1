//includes nodig voor functies
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16E6 // used in _delay_ms, 16 MHz
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/sfr_defs.h>

//variable declaration
volatile uint16_t requestingdistance = 0;
volatile uint16_t busy = 0;
volatile uint16_t gotresult = 0;
volatile uint16_t distance = 0;
volatile uint8_t overflow = 0;
volatile uint32_t timervalue;
volatile uint8_t echostatus = 1;



//Deze methode zet de triggerpin op high, wacht even en zet hem dan weer op low
void fire_echo()
{
	_delay_ms(50);
	//_delay_us(1);
	PORTD |= (1 << PIND4); //
	_delay_us(12);         // Stuur ~10 seconden pulse
	PORTD = 0x00;          //
	_delay_ms(50);
}

ISR (INT1_vect)
{
	if (echostatus == 0)
	{
		TCNT1 = 0;
		overflow = 0;
		TCCR1B |= (1<<CS10) | (0<<CS11) | (1<<CS12); //De prescaler voor de timer!
		echostatus = 1;
	}
	else if (echostatus == 1)
	{
		TCCR1B = 0;
		timervalue = TCNT1; //Het meegeven van het TCNT1 register aan timervalue
		gotresult = 1;
	}
}

ISR (TIMER1_OVF_vect)
{
	overflow ++;
}

void setEcho(int echostat){
	echostatus = echostat;
}