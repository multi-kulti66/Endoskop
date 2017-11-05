#include "Stepper.h"


Stepper::Stepper(AccelStepper &stepper) : _stepper(stepper)
{
	_stepper.setMaxSpeed(MAX_SPEED);
	setCurrentPosition(0);
}


void Stepper::step() const
{
	_stepper.runSpeedToPosition();
}


boolean Stepper::setForwardMovement(const float speed) const
{
	if(isRunning())
	{
		return false;
	}

	_stepper.move(1);
	_stepper.setSpeed(speed);
	return true;
}


boolean Stepper::setBackwardMovement(const float speed) const
{
	if(isRunning())
	{
		return false;
	}

	_stepper.move(-1);
	_stepper.setSpeed(speed);
	return true;
}


void Stepper::setCurrentPosition(const long position) const
{
	_stepper.setCurrentPosition(position);
}


long Stepper::getCurrentPosition() const
{
	return _stepper.currentPosition();
}


long Stepper::getTargetPosition() const
{
	return _stepper.targetPosition();
}


bool Stepper::isRunning() const
{
	if(_stepper.distanceToGo() != 0)
	{
		return true;
	}

	return false;
}
