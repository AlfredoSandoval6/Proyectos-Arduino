#ifndef Parpadeo_h
#define Parpadeo_h
#include "Arduino.h"

class Parpadeo{

	public:

	Parpadeo(int pin);
	void light();

	private:

	int _pin;
	int _i;

};

#endif