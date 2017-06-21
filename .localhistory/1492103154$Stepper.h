#ifndef STEPPER_H
#define STEPPER_H

#include "Arduino.h"
#include "AccelStepper.h"


class Stepper
{
public:
	/* Constructors */
	Stepper(AccelStepper& stepper);

	/* Methods */
	void step() const;
	void setForwardMovement(uint8_t steps, float speed) const;
	void setBackwardMovement(uint8_t steps, float speed) const;
	void setCurrentPosition(long position) const;
	long getCurrentPosition() const;
	long getTargetPosition() const;
	bool isRunning() const;

private:
	/* Constants */
	const uint16_t MAX_SPEED = 500;
	const float DELTA_ZERO = 0.1;

	/* References */
	AccelStepper& _stepper;
};

#endif // STEPPER_H
