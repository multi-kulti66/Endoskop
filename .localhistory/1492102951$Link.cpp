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
Link::Link(Stepper& stepperUp, Stepper& stepperRight, Stepper& stepperDown, Stepper& stepperLeft,
           LimitBarrier& limitBarrierUp, LimitBarrier& limitBarrierRight, LimitBarrier& limitBarrierDown, LimitBarrier& limitBarrierLeft) : _stepperUp(stepperUp), _stepperRight(stepperRight), _stepperDown(stepperDown), _stepperLeft(stepperLeft),
                                                                                                                                            _limitBarrierUp(limitBarrierUp), _limitBarrierRight(limitBarrierRight), _limitBarrierDown(limitBarrierDown), _limitBarrierLeft(limitBarrierLeft)
{}


/**
 * \brief Indicates whether all steppers have reached their limit barrier.
 * \return true = all steppers have reached limit barriers
 */
boolean Link::haveReachedLimitBarriersForInit() const
{
	// TODO: wieder auskommentieren
	/*if(_limitBarrierUp.hasReachedBarrier() && _limitBarrierRight.hasReachedBarrier() && _limitBarrierDown.hasReachedBarrier() && _limitBarrierLeft.hasReachedBarrier())
	{
		return true;
	}*/

	if(_limitBarrierUp.hasReachedBarrier())
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
	// reset that limit will not be reached accidentally
	setStepperPositionsForInit(0);

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
void Link::setHorizontalDirectionMovement(HorizontalDirection horizontalDirection) const
{
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
void Link::setVerticalDirectionMovement(VerticalDirection verticalDirection) const
{
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
void Link::setStepperPositionsForInit(long position) const
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
boolean Link::hasReachedPositiveEndPosition(Stepper& stepper) const
{
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
boolean Link::hasReachedNegativeEndPosition(Stepper& stepper) const
{
	if(stepper.getCurrentPosition() <= (-MAX_POSITION))
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
boolean Link::isInPositivePosition(Stepper& stepper) const
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
void Link::prepareForForwardMovement(Stepper& stepper, LimitBarrier& limitBarrier) const
{
	if(hasReachedPositiveEndPosition(stepper) || limitBarrier.hasReachedBarrier())
	{
		return;
	}

	if(isInPositivePosition(stepper))
	{
		stepper.setForwardMovement(STEPS_FAST, SPEED_FAST);
	}
	else
	{
		stepper.setForwardMovement(STEPS_SLOW, SPEED_SLOW);
	}
}


/**
* \brief	Initializes the stepper for the next movement in the negative direction.
*			It will set the speed depending on the current position.
* \param stepper	The stepper that speed should be set.
*/
void Link::prepareForBackwardMovement(Stepper& stepper) const
{
	if(hasReachedNegativeEndPosition(stepper))
	{
		return;
	}

	if(isInPositivePosition(stepper))
	{
		stepper.setBackwardMovement(STEPS_FAST, SPEED_FAST);
	}
	else
	{
		stepper.setBackwardMovement(STEPS_SLOW, SPEED_SLOW);
	}
}
