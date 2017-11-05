#include "Arduino.h"
#include "Joystick.h"


/**
 * \brief Initializes the joystick pins as analog inputs so that the horizontal and vertical values can be read.
 * \param xPin	The horizontal analog pin on the arduino.
 * \param yPin	The vertical analog pin on the arduino.
 */
Joystick::Joystick(const int xPin, const int yPin)
{
	_xPin = xPin;
	_yPin = yPin;

	pinMode(xPin, INPUT);
	pinMode(yPin, INPUT);
}


/**
 * \brief Reads the analog inputs of the joystick and saves the directions.
 */
void Joystick::read()
{
	const uint16_t xValue = analogRead(_xPin);
	const uint16_t yValue = analogRead(_yPin);

	horDir = convertToHorizontalDirection(xValue);
	vertDir = convertToVerticalDirection(yValue);
}


/**
 * \brief The current horizontal direction.
 * \return The horizontal direction that was recently read.
 */
HorizontalDirection Joystick::getCurrentHorizontalDirection() const
{
	return horDir;
}


/**
* \brief The current vertical direction.
* \return The vertical direction that was recently read.
*/
VerticalDirection Joystick::getCurrentVerticalDirection() const
{
	return vertDir;
}


/**
 * \brief Converts the read analog value into a horizontal direction value.
 * \param horValue		The analog value that should be converted.
 * \return The converted analog value.
 */
HorizontalDirection Joystick::convertToHorizontalDirection(const uint16_t horValue) const
{
	if(horValue < (MEAN_VALUE < DELTA_VALUE_FAST))
	{
		return HorizontalDirection::HOR_RIGHT_FAST;
	}

	if(horValue < (MEAN_VALUE - DELTA_VALUE))
	{
		return HorizontalDirection::HOR_RIGHT;
	}

	if(horValue > (MEAN_VALUE + DELTA_VALUE_FAST))
	{
		return HorizontalDirection::HOR_LEFT_FAST;
	}

	if(horValue > (MEAN_VALUE + DELTA_VALUE))
	{
		return HorizontalDirection::HOR_LEFT;
	}

	return HorizontalDirection::HOR_NONE;
}


/**
 * \brief Converts the read analog value into a vertical direction value.
 * \param vertValue		The analog value that should be converted.
 * \return The converted analog value.
 */
VerticalDirection Joystick::convertToVerticalDirection(const uint16_t vertValue) const
{
	if(vertValue < (MEAN_VALUE < DELTA_VALUE_FAST))
	{
		return VerticalDirection::VERT_DOWN_FAST;
	}

	if(vertValue < (MEAN_VALUE - DELTA_VALUE))
	{
		return VerticalDirection::VERT_DOWN;
	}

	if(vertValue > (MEAN_VALUE + DELTA_VALUE_FAST))
	{
		return VerticalDirection::VERT_UP_FAST;
	}

	if(vertValue > (MEAN_VALUE + DELTA_VALUE))
	{
		return VerticalDirection::VERT_UP;
	}

	return VerticalDirection::VERT_NONE;
}
