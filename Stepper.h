#ifndef STEPPER_H
#define STEPPER_H

#include "AccelStepper.h"



class Stepper
{
public:
	/* Constructors */
	Stepper(AccelStepper &stepper);

	/* Methods */
	void step() const;
	boolean setForwardMovement(const float speed) const;
	boolean setBackwardMovement(const float speed) const;
	void setCurrentPosition(const long position) const;
	long getCurrentPosition() const;
	long getTargetPosition() const;
	bool isRunning() const;

private:
	/* Constants */
	const float MAX_SPEED = 750;

	/* References */
	AccelStepper &_stepper;
};

#endif // STEPPER_H
