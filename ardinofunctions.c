// Pin functions #bron: gebaseerd op functie arduino C
void setPinMode(char type, int pin, uint8_t val) {
	switch(type) {
		case 'D':
		if(pin >= 8) {
			pin -= 8;
			if (val == LOW) {
				DDRB &= ~(_BV(pin)); // clear bit
				} else {
				DDRB |= _BV(pin); // set bit
			}
			} else {
			if (val == LOW) {
				DDRD &= ~(_BV(pin)); // clear bit
				} else {
				DDRD |= _BV(pin); // set bit
			}
		}

		break;

		case 'A':
		case 'C':
		if (val == LOW) {
			DDRC &= ~(_BV(pin)); // clear bit
			} else {
			DDRC |= _BV(pin); // set bit
		}
		break;
	}
}
// #bron: gebaseerd op functie arduino C
void setPinVal(char type, int pin, uint8_t val) {
	switch(type) {
		case 'D':
		if(pin >= 8) {
			pin -= 8;
			if (val == LOW) {
				PORTB &= ~(_BV(pin)); // clear bit
				} else {
				PORTB |= _BV(pin); // set bit
			}
			} else {
			if (val == LOW) {
				PORTD &= ~(_BV(pin)); // clear bit
				} else {
				PORTD |= _BV(pin); // set bit
			}
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
