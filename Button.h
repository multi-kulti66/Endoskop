#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"



class Button
{
public:
	/* Constructors */
	Button(const int pin);


	/* Methods */
	boolean isButtonPressed() const;

private:
	/* Variables */
	int _pin; // Represents the assigned pin on the arduino.
};

#endif // BUTTON_H
