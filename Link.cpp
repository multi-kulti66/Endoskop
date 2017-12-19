#include "Arduino.h"
#include "Link.h"


Link::Link(Stepper &stepperUp, Stepper &stepperRight, Stepper &stepperDown, Stepper &stepperLeft,
           LimitBarrier &limitBarrierUp, LimitBarrier &limitBarrierRight, LimitBarrier &limitBarrierDown,
           LimitBarrier &limitBarrierLeft)
	: _stepperUp(stepperUp), _stepperRight(stepperRight), _stepperDown(stepperDown), _stepperLeft(stepperLeft),
	  _limitBarrierUp(limitBarrierUp), _limitBarrierRight(limitBarrierRight), _limitBarrierDown(limitBarrierDown),
	  _limitBarrierLeft(limitBarrierLeft)
{}


boolean Link::haveReachedLimitBarriersForInit() const
{
	if(_limitBarrierUp.hasReachedBarrier() && _limitBarrierRight.hasReachedBarrier() &&
		_limitBarrierDown.hasReachedBarrier() && _limitBarrierLeft.hasReachedBarrier())
	{
		return true;
	}

	return false;
}


void Link::setMovementsToLimitBarrierForInit() const
{
	// reset that max position will not be reached accidentally
	// -1 so that pos_neg_factor has no influence
	setStepperPositionsForInit(-1);

	prepareForFastForwardMovement(_stepperUp, _limitBarrierUp);
	prepareForFastForwardMovement(_stepperRight, _limitBarrierRight);
	prepareForFastForwardMovement(_stepperDown, _limitBarrierDown);
	prepareForFastForwardMovement(_stepperLeft, _limitBarrierLeft);
}


boolean Link::isCenteredForInit() const
{
	if(limitToCenterCounter >= POS_MAX_POSITION)
	{
		setStepperPositionsForInit(0);
		return true;
	}

	return false;
}


void Link::setMovementsToCenterForInit()
{
	if(isMoving())
	{
		return;
	}

	// set position the way that pos_neg_factor has no influence
	setStepperPositionsForInit(-1);
	prepareForFastBackwardMovement(_stepperUp);
	prepareForFastBackwardMovement(_stepperRight);
	prepareForFastBackwardMovement(_stepperDown);
	prepareForFastBackwardMovement(_stepperLeft);
	limitToCenterCounter++;
}


void Link::setHorizontalDirectionMovement(const HorizontalDirection horizontalDirection) const
{
	if(horizontalDirection == HorizontalDirection::HOR_RIGHT_FAST)
	{
		prepareForFastBackwardMovement(_stepperRight);
		prepareForFastForwardMovement(_stepperLeft, _limitBarrierLeft);
	}
	if(horizontalDirection == HorizontalDirection::HOR_RIGHT)
	{
		prepareForBackwardMovement(_stepperRight);
		prepareForForwardMovement(_stepperLeft, _limitBarrierLeft);
	}
	else if(horizontalDirection == HorizontalDirection::HOR_LEFT_FAST)
	{
		prepareForFastForwardMovement(_stepperRight, _limitBarrierRight);
		prepareForFastBackwardMovement(_stepperLeft);
	}
	else if(horizontalDirection == HorizontalDirection::HOR_LEFT)
	{
		prepareForForwardMovement(_stepperRight, _limitBarrierRight);
		prepareForBackwardMovement(_stepperLeft);
	}
	else
	{
		// do nothing, no direction selected
	}
}


void Link::setVerticalDirectionMovement(const VerticalDirection verticalDirection) const
{
	if(verticalDirection == VerticalDirection::VERT_UP_FAST)
	{
		prepareForFastBackwardMovement(_stepperUp);
		prepareForFastForwardMovement(_stepperDown, _limitBarrierDown);
	}
	else if(verticalDirection == VerticalDirection::VERT_UP)
	{
		prepareForBackwardMovement(_stepperUp);
		prepareForForwardMovement(_stepperDown, _limitBarrierDown);
	}
	else if(verticalDirection == VerticalDirection::VERT_DOWN_FAST)
	{
		prepareForFastForwardMovement(_stepperUp, _limitBarrierUp);
		prepareForFastBackwardMovement(_stepperDown);
	}
	else if(verticalDirection == VerticalDirection::VERT_DOWN)
	{
		prepareForForwardMovement(_stepperUp, _limitBarrierUp);
		prepareForBackwardMovement(_stepperDown);
	}
	else
	{
		// do nothing, no direction selected
	}
}


boolean Link::isMoving() const
{
	if(_stepperUp.isRunning() || _stepperRight.isRunning() || _stepperDown.isRunning() || _stepperLeft.isRunning())
	{
		return true;
	}

	return false;
}


void Link::update() const
{
	_stepperUp.step();
	_stepperRight.step();
	_stepperDown.step();
	_stepperLeft.step();
}


void Link::setStepperPositionsForInit(const long position) const
{
	_stepperUp.setCurrentPosition(position);
	_stepperRight.setCurrentPosition(position);
	_stepperDown.setCurrentPosition(position);
	_stepperLeft.setCurrentPosition(position);
}


boolean Link::hasReachedPositiveEndPosition(Stepper &stepper) const
{
	if(stepper.getCurrentPosition() >= POS_MAX_POSITION)
	{
		return true;
	}

	return false;
}


boolean Link::hasReachedNegativeEndPosition(Stepper &stepper) const
{
	if(stepper.getCurrentPosition() <= NEG_MAX_POSITION)
	{
		return true;
	}

	return false;
}


boolean Link::isInPositivePosition(Stepper &stepper) const
{
	if(stepper.getCurrentPosition() >= 0)
	{
		return true;
	}

	return false;
}


boolean Link::prepareForFastForwardMovement(Stepper &stepper, LimitBarrier &limitBarrier) const
{
	if(hasReachedPositiveEndPosition(stepper) || limitBarrier.hasReachedBarrier())
	{
		return false;
	}

	if(isInPositivePosition(stepper))
	{
		return stepper.setForwardMovement(SPEED_FAST * POS_NEG_SPEED_FACTOR);
	}

	return stepper.setForwardMovement(SPEED_FAST);
}


boolean Link::prepareForForwardMovement(Stepper &stepper, LimitBarrier &limitBarrier) const
{
	if(hasReachedPositiveEndPosition(stepper) || limitBarrier.hasReachedBarrier())
	{
		return false;
	}

	if(isInPositivePosition(stepper))
	{
		return stepper.setForwardMovement(SPEED_SLOW * POS_NEG_SPEED_FACTOR);
	}

	return stepper.setForwardMovement(SPEED_SLOW);
}


boolean Link::prepareForFastBackwardMovement(Stepper &stepper) const
{
	if(hasReachedNegativeEndPosition(stepper))
	{
		return false;
	}

	if(isInPositivePosition(stepper))
	{
		return stepper.setBackwardMovement(SPEED_FAST * POS_NEG_SPEED_FACTOR);
	}

	return stepper.setBackwardMovement(SPEED_FAST);
}


boolean Link::prepareForBackwardMovement(Stepper &stepper) const
{
	if(hasReachedNegativeEndPosition(stepper))
	{
		return false;
	}

	if(isInPositivePosition(stepper))
	{
		return stepper.setBackwardMovement(SPEED_SLOW * POS_NEG_SPEED_FACTOR);
	}

	return stepper.setBackwardMovement(SPEED_SLOW);
}
