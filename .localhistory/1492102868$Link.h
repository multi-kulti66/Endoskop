#ifndef LINK_H
#define LINK_H

#include "Arduino.h"
#include "HorizontalDirection.h"
#include "VerticalDirection.h"
#include "Stepper.h"
#include "LimitBarrier.h"



class Link
{
public:
	/* Constructors */
	Link(Stepper &stepperUp, Stepper &stepperRight, Stepper &stepperDown, Stepper &stepperLeft,
	     LimitBarrier &limitBarrierUp, LimitBarrier &limitBarrierRight, LimitBarrier &limitBarrierDown, LimitBarrier &limitBarrierLeft);


	/* Methods */
	boolean haveReachedLimitBarriersForInit() const;
	void setMovementsToLimitBarrierForInit() const;
	boolean isCenteredForInit() const;
	void setMovementsToCenterForInit();
	void setHorizontalDirectionMovement(HorizontalDirection horizontalDirection) const;
	void setVerticalDirectionMovement(VerticalDirection verticalDirection) const;
	boolean isMoving() const;
	void update() const;

private:
	/* Constants */
	const long STEPS_BARRIER_TO_CENTER = 1000;
	const long MAX_POSITION = STEPS_BARRIER_TO_CENTER * 2;

	const uint16_t SPEED_SLOW = 250;
	const uint16_t SPEED_FAST = 500;

	const uint8_t STEPS_SLOW = 1;
	const uint8_t STEPS_FAST = 2;

	/* Variables */
	long limitToCenterCounter = 0;

	/* Components */
	Stepper &_stepperUp;
	Stepper &_stepperRight;
	Stepper &_stepperDown;
	Stepper &_stepperLeft;
	LimitBarrier &_limitBarrierUp;
	LimitBarrier &_limitBarrierRight;
	LimitBarrier &_limitBarrierDown;
	LimitBarrier &_limitBarrierLeft;

	/* Methods */
	void setStepperPositionsForInit(long position) const;
	boolean hasReachedPositiveEndPosition(Stepper &stepper) const;
	boolean hasReachedNegativeEndPosition(Stepper &stepper) const;
	boolean isInPositivePosition(Stepper &stepper) const;
	void prepareForForwardMovement(Stepper &stepper, LimitBarrier &limitBarrier) const;
	void prepareForBackwardMovement(Stepper &stepper) const;
};

#endif
