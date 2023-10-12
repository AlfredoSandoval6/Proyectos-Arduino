#include "Arduino.h"
#include "Parpadeo.h"

Parpadeo::Parpadeo(int pin){

	pinMode(pin, OUTPUT);
	_pin = pin;

}

void Parpadeo::light(){

	while(_i < 3){

		digitalWrite(_pin, HIGH);
		delay(100);
		digitalWrite(_pin, LOW);
		delay(100);

	}

	_i = 0;

	delay(500);

}