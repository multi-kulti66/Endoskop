#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "Arduino.h"
#include "HorizontalDirection.h"
#include "VerticalDirection.h"


class Joystick
{
public:
	/* Constructors */
	Joystick(int xPin, int yPin);

	/* Methods */
	void read();
	HorizontalDirection getCurrentHorizontalDirection() const;
	VerticalDirection getCurrentVerticalDirection() const;

private:
	/* Constants */
	const uint16_t MEAN_VALUE = 520;
	const uint16_t DELTA_VALUE = 20;

	/* Variables */
	int _xPin; // analog pin for the horizontal value
	int _yPin; // analog pin for the vertical value
	HorizontalDirection horDir = HorizontalDirection::HOR_NONE;
	VerticalDirection vertDir = VerticalDirection::VERT_NONE;

	/* Methods */
	HorizontalDirection convertToHorizontalDirection(uint16_t horValue) const;
	VerticalDirection convertToVerticalDirection(uint16_t vertValue) const;
};

#endif // JOYSTICK_H
