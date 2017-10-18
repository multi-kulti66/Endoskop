#ifndef STEPPER_H
#define STEPPER_H

#include "Arduino.h"
#include "AccelStepper.h"



class Stepper
{
public:
	/* Constructors */
	Stepper(AccelStepper &stepper);

	/* Methods */
	void step() const;
	void setForwardMovement(const uint8_t steps, const float speed) const;
	void setBackwardMovement(const uint8_t steps, const float speed) const;
	void setCurrentPosition(const long position) const;
	long getCurrentPosition() const;
	long getTargetPosition() const;
	bool isRunning() const;

private:
	/* Constants */
	const uint16_t MAX_SPEED = 500;
	const float DELTA_ZERO = 0.1;

	/* References */
	AccelStepper &_stepper;
};

#endif // STEPPER_H
