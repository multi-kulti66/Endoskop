#include "Arduino.h"
#include "Link.h"


/**
 * \brief Assigns the steppers to the link.
 * \param stepperUp				The stepper for the upwards movement.
 * \param stepperRight			The stepper for the rightwards movement.
 * \param stepperDown			The stepper for the downwards movement.
 * \param stepperLeft			The stepper for the leftwards movement.
 * \param limitBarrierUp		The limit barrier for the upwards movement.
 * \param limitBarrierRight		The limit barrier for the rightwards movement.
 * \param limitBarrierDown		The limit barrier for the downwards movement.
 * \param limitBarrierLeft		The limit barrier for the leftwards movement.
 */
Link::Link(Stepper &stepperUp, Stepper &stepperRight, Stepper &stepperDown, Stepper &stepperLeft,
           LimitBarrier &limitBarrierUp, LimitBarrier &limitBarrierRight, LimitBarrier &limitBarrierDown,
           LimitBarrier &limitBarrierLeft)
	: _stepperUp(stepperUp), _stepperRight(stepperRight), _stepperDown(stepperDown), _stepperLeft(stepperLeft),
	  _limitBarrierUp(limitBarrierUp), _limitBarrierRight(limitBarrierRight), _limitBarrierDown(limitBarrierDown),
	  _limitBarrierLeft(limitBarrierLeft)
{}


/**
 * \brief Indicates whether all steppers have reached their limit barrier.
 * \return true = all steppers have reached limit barriers
 */
boolean Link::haveReachedLimitBarriersForInit() const
{
	if(_limitBarrierUp.hasReachedBarrier() && _limitBarrierRight.hasReachedBarrier() &&
		_limitBarrierDown.hasReachedBarrier() && _limitBarrierLeft.hasReachedBarrier())
	{
		return true;
	}

	return false;
}


/**
 * \brief Initializes all steppers for the next movement towards the limit barrier.
 */
void Link::setMovementsToLimitBarrierForInit() const
{
	// TODO: position really 0?
	// reset that max position will not be reached accidentally
	setStepperPositionsForInit(0);

	// TODO: all for forward movement?
	prepareForForwardMovement(_stepperUp, _limitBarrierUp);
	prepareForForwardMovement(_stepperRight, _limitBarrierRight);
	prepareForForwardMovement(_stepperDown, _limitBarrierDown);
	prepareForForwardMovement(_stepperLeft, _limitBarrierLeft);
}


/**
 * \brief Indicates whether the link has been centered.
 * \return true = all links have been centered
 */
boolean Link::isCenteredForInit() const
{
	if(limitToCenterCounter >= STEPS_BARRIER_TO_CENTER)
	{
		return true;
	}

	return false;
}


/**
 * \brief Initializes all steppers for the next movement towards the center.
 */
void Link::setMovementsToCenterForInit()
{
	// after movement stepper position = 0
	setStepperPositionsForInit(STEPS_FAST);

	//TODO: rethink movements
	// TODO: check if center position reached? (STEPS_BARRIER_TO_CENTER)
	prepareForBackwardMovement(_stepperUp);
	prepareForBackwardMovement(_stepperRight);
	prepareForBackwardMovement(_stepperDown);
	prepareForBackwardMovement(_stepperLeft);

	limitToCenterCounter++;
}


/**
 * \brief Initializes the steppers for the next horizontal movement.
 * \param horizontalDirection	The horizontal direction in which the link should move.
 */
void Link::setHorizontalDirectionMovement(const HorizontalDirection horizontalDirection) const
{
	// TODO: are directions correct?
	if(horizontalDirection == HorizontalDirection::HOR_RIGHT)
	{
		prepareForBackwardMovement(_stepperRight);
		prepareForForwardMovement(_stepperLeft, _limitBarrierLeft);
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


/**
 * \brief Initializes the steppers for the next vertical movement.
 * \param verticalDirection		The vertical direction in which the link should move.
 */
void Link::setVerticalDirectionMovement(const VerticalDirection verticalDirection) const
{
	// TODO: are directions correct?
	if(verticalDirection == VerticalDirection::VERT_UP)
	{
		prepareForBackwardMovement(_stepperUp);
		prepareForForwardMovement(_stepperDown, _limitBarrierDown);
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


/**
 * \brief Indicates whether a stepper is still running.
 * \return true = at least one stepper is still running
 */
boolean Link::isMoving() const
{
	if(_stepperUp.isRunning() || _stepperRight.isRunning() || _stepperDown.isRunning() || _stepperLeft.isRunning())
	{
		return true;
	}

	return false;
}


/**
 * \brief Updates the current state of the steppers and let them step if necessary.
 * \note This method must be called frequently to get the steppers moving.
 */
void Link::update() const
{
	_stepperUp.step();
	_stepperRight.step();
	_stepperDown.step();
	_stepperLeft.step();
}


/**
 * \brief Sets all current positions and target positions.
 * \param position	The new stepper position
 */
void Link::setStepperPositionsForInit(const long position) const
{
	_stepperUp.setCurrentPosition(position);
	_stepperRight.setCurrentPosition(position);
	_stepperDown.setCurrentPosition(position);
	_stepperLeft.setCurrentPosition(position);
}


/**
 * \brief Indicates wether the stepper has reached the allowed maximum positive position.
 * \param stepper	The stepper that position should be checked
 * \return true = has reached end position
 */
boolean Link::hasReachedPositiveEndPosition(Stepper &stepper) const
{
	// TODO: actually check limit barrier state (maybe for reinit current position -> when stepper overtwisted)
	if(stepper.getCurrentPosition() >= MAX_POSITION)
	{
		return true;
	}

	return false;
}


/**
* \brief Indicates wether the stepper has reached the allowed maximum negative position.
* \param stepper	The stepper that position should be checked
* \return true = has reached end position
*/
boolean Link::hasReachedNegativeEndPosition(Stepper &stepper) const
{
	if(stepper.getCurrentPosition() <= -MAX_POSITION)
	{
		return true;
	}

	return false;
}


/**
 * \brief Indicates whether the stepper position is currently positive.
 * \param stepper	The stepper which position should be checked.
 * \return true = is in positive position
 */
boolean Link::isInPositivePosition(Stepper &stepper) const
{
	if(stepper.getCurrentPosition() >= 0)
	{
		return true;
	}

	return false;
}


/**
* \brief	Initializes the stepper for the next movement in the positive direction.
*			It will set the speed depending on the current position.
* \param stepper	The stepper that speed should be set.
*/
void Link::prepareForForwardMovement(Stepper &stepper, LimitBarrier &limitBarrier) const
{
	// TODO: comment out when end stoppers are connected, otherwise floating values
	/*if(hasReachedPositiveEndPosition(stepper) || limitBarrier.hasReachedBarrier())
	{
		return;
	}*/

	// TODO: rethink speeds in positive / negative direction -> what on init? better slow (set position before)
	/*if(isInPositivePosition(stepper))
	{
		stepper.setForwardMovement(STEPS_FAST, SPEED_FAST);
	}
	else
	{
		stepper.setForwardMovement(STEPS_SLOW, SPEED_SLOW);
	}*/

	stepper.setForwardMovement(STEPS_FAST, SPEED_FAST);
}


/**
* \brief	Initializes the stepper for the next movement in the negative direction.
*			It will set the speed depending on the current position.
* \param stepper	The stepper that speed should be set.
*/
void Link::prepareForBackwardMovement(Stepper &stepper) const
{
	/*if(hasReachedNegativeEndPosition(stepper))
	{
		return;
	}*/

	// TODO: comment out, if different speeds in positive / negative position
	/*if(isInPositivePosition(stepper))
	{
		stepper.setBackwardMovement(STEPS_FAST, SPEED_FAST);
	}
	else
	{
		stepper.setBackwardMovement(STEPS_SLOW, SPEED_SLOW);
	}*/

	stepper.setBackwardMovement(STEPS_FAST, SPEED_FAST);
}
