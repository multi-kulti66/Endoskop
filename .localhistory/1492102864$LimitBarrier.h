#ifndef LIMIT_BARRIER_H
#define LIMIT_BARRIER_H

#include "Arduino.h"



class LimitBarrier
{
public:
	/* Constructors */
	LimitBarrier(uint8_t pin);

	/* Methods */
	boolean hasReachedBarrier() const;

private:
	/* Variables */
	uint8_t _pin; // Represents the assigned pin on the arduino.
};

#endif // LIMIT_BARRIER_H
