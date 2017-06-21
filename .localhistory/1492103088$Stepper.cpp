#include "Stepper.h"


/**
 * \brief Initializes the stepper for usage via stepper driver.
 * \param stepper	The reference to the AccelStepper instance.
 */
Stepper::Stepper(AccelStepper &stepper) : _stepper(stepper)
{
	_stepper.setMaxSpeed(MAX_SPEED);
	setCurrentPosition(0);
}


/**
 * \brief Steps to the target position.
 * \note If target position equals current position or speed equals zero nothing happens.
 */
void Stepper::step() const
{
	_stepper.runSpeedToPosition();
}


/**
* \brief Set new target position in forward direction.
* \param steps		The number of steps that should be moved.
* \param speed		The speed in which the steps should be made.
*/
void Stepper::setForwardMovement(uint8_t steps, float speed) const
{
	_stepper.move(steps);
	_stepper.setSpeed(speed);
}


/**
 * \brief Set new target position in backward direction.
 * \param steps		The number of steps that should be moved.
 * \param speed		The speed in which the steps should be made.
 */
void Stepper::setBackwardMovement(uint8_t steps, float speed) const
{
	_stepper.move(-steps);
	_stepper.setSpeed(speed);
}


/**
* \brief Set the current position and target position.
*/
void Stepper::setCurrentPosition(long position) const
{
	_stepper.setCurrentPosition(position);
}


/**
 * \brief The current position.
 * \return The current position in steps.
 */
long Stepper::getCurrentPosition() const
{
	return _stepper.currentPosition();
}


/**
 * \brief The target position.
 * \return The target position in steps.
 */
long Stepper::getTargetPosition() const
{
	return _stepper.targetPosition();
}


/**
 * \brief Indicates whether the target position was reached.
 * \return true = target position equals current position
 */
bool Stepper::isRunning() const
{
	if(_stepper.currentPosition() != _stepper.targetPosition())
	{
		return true;
	}

	return false;
}
