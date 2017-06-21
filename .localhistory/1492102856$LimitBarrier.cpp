#include "Arduino.h"
#include "LimitBarrier.h"


/**
 * \brief Assigns the pin and sets it to input mode.
 * \param pin	The digital pin value on the arduino.
 */
LimitBarrier::LimitBarrier(uint8_t pin)
{
	this->_pin = pin;
	pinMode(_pin, INPUT);
}


/**
 * \brief Indicates whether the limit barrier is reached.
 * \return true = pressed
 */
boolean LimitBarrier::hasReachedBarrier() const
{
	return digitalRead(this->_pin);
}
