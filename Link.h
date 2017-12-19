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
	     LimitBarrier &limitBarrierUp, LimitBarrier &limitBarrierRight, LimitBarrier &limitBarrierDown,
	     LimitBarrier &limitBarrierLeft);


	/* Methods */
	boolean haveReachedLimitBarriersForInit() const;
	void setMovementsToLimitBarrierForInit() const;
	boolean isCenteredForInit() const;
	void setMovementsToCenterForInit();
	void setHorizontalDirectionMovement(const HorizontalDirection horizontalDirection) const;
	void setVerticalDirectionMovement(const VerticalDirection verticalDirection) const;
	boolean isMoving() const;
	void update() const;

private:
	/* Constants */
	const float SPEED_SLOW = 250;
	const float SPEED_FAST = 500;
	const float POS_NEG_SPEED_FACTOR = 1.25;

	const long POS_MAX_POSITION = 1600;
	const long NEG_MAX_POSITION = -static_cast<float>(POS_MAX_POSITION) / POS_NEG_SPEED_FACTOR;


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
	void setStepperPositionsForInit(const long position) const;
	boolean hasReachedPositiveEndPosition(Stepper &stepper) const;
	boolean hasReachedNegativeEndPosition(Stepper &stepper) const;
	boolean isInPositivePosition(Stepper &stepper) const;
	boolean prepareForFastForwardMovement(Stepper &stepper, LimitBarrier &limitBarrier) const;
	boolean prepareForForwardMovement(Stepper &stepper, LimitBarrier &limitBarrier) const;
	boolean prepareForFastBackwardMovement(Stepper &stepper) const;
	boolean prepareForBackwardMovement(Stepper &stepper) const;
};

#endif
