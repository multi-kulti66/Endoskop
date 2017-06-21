#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"



class Button
{
public:
	/* Constructors */
	Button(uint8_t pin);


	/* Methods */
	boolean isButtonPressed() const;

private:
	/* Variables */
	uint8_t _pin; // Represents the assigned pin on the arduino.
};

#endif // BUTTON_H
