#include "Arduino.h"
#include "Button.h"


/**
 * \brief Assigns the pin and sets it to input mode.
 * \param pin	The digital pin value on the arduino.
 */
Button::Button(const uint8_t pin)
{
	_pin = pin;
	pinMode(_pin, INPUT);
}


/**
 * \brief Indicates whether the button is pressed.
 * \return true = pressed
 */
boolean Button::isButtonPressed() const
{
	return digitalRead(_pin);
}
