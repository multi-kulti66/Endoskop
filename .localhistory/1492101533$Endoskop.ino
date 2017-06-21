#include "Arduino.h"
#include "Joystick.h"
#include "Button.h"
#include "AccelStepper.h"
#include "Stepper.h"
#include "LimitBarrier.h"
#include "Link.h"

/* Constants */
const uint8_t NUMBER_OF_LINKS = 1;
const uint8_t NUMBER_OF_STEPPERS = NUMBER_OF_LINKS * 4;


/* Components */
Joystick joystick(A0, A1);

Button button1(0);
Button button2(1);
Button button3(2);
Button button4(3);

Button *buttons[NUMBER_OF_LINKS] = {
		&button1
	};


AccelStepper accelStepper1(AccelStepper::MotorInterfaceType::DRIVER, 4, 5);
Stepper stepper1(accelStepper1);
LimitBarrier limitBarrier1(6);

AccelStepper accelStepper2(AccelStepper::MotorInterfaceType::DRIVER, 7, 8);
Stepper stepper2(accelStepper2);
LimitBarrier limitBarrier2(9);

AccelStepper accelStepper3(AccelStepper::MotorInterfaceType::DRIVER, 10, 11);
Stepper stepper3(accelStepper3);
LimitBarrier limitBarrier3(12);

AccelStepper accelStepper4(AccelStepper::MotorInterfaceType::DRIVER, 13, 14);
Stepper stepper4(accelStepper4);
LimitBarrier limitBarrier4(15);

//AccelStepper accelStepper5(AccelStepper::MotorInterfaceType::DRIVER, 16, 17);
//Stepper stepper5(accelStepper5);
//LimitBarrier limitBarrier5(18);
//
//AccelStepper accelStepper6(AccelStepper::MotorInterfaceType::DRIVER, 19, 20);
//Stepper stepper6(accelStepper6);
//LimitBarrier limitBarrier6(21);
//
//AccelStepper accelStepper7(AccelStepper::MotorInterfaceType::DRIVER, 22, 23);
//Stepper stepper7(accelStepper7);
//LimitBarrier limitBarrier7(24);
//
//AccelStepper accelStepper8(AccelStepper::MotorInterfaceType::DRIVER, 25, 26);
//Stepper stepper8(accelStepper8);
//LimitBarrier limitBarrier8(27);
//
//AccelStepper accelStepper9(AccelStepper::MotorInterfaceType::DRIVER, 28, 29);
//Stepper stepper9(accelStepper9);
//LimitBarrier limitBarrier9(30);
//
//AccelStepper accelStepper10(AccelStepper::MotorInterfaceType::DRIVER, 31, 32);
//Stepper stepper10(accelStepper10);
//LimitBarrier limitBarrier10(33);
//
//AccelStepper accelStepper11(AccelStepper::MotorInterfaceType::DRIVER, 34, 35);
//Stepper stepper11(accelStepper11);
//LimitBarrier limitBarrier11(36);
//
//AccelStepper accelStepper12(AccelStepper::MotorInterfaceType::DRIVER, 37, 38);
//Stepper stepper12(accelStepper12);
//LimitBarrier limitBarrier12(39);
//
//AccelStepper accelStepper13(AccelStepper::MotorInterfaceType::DRIVER, 40, 41);
//Stepper stepper13(accelStepper13);
//LimitBarrier limitBarrier13(42);
//
//AccelStepper accelStepper14(AccelStepper::MotorInterfaceType::DRIVER, 43, 44);
//Stepper stepper14(accelStepper14);
//LimitBarrier limitBarrier14(45);
//
//AccelStepper accelStepper15(AccelStepper::MotorInterfaceType::DRIVER, 46, 47);
//Stepper stepper15(accelStepper15);
//LimitBarrier limitBarrier15(48);
//
//AccelStepper accelStepper16(AccelStepper::MotorInterfaceType::DRIVER, 49, 50);
//Stepper stepper16(accelStepper16);
//LimitBarrier limitBarrier16(51);

Stepper *steppers[NUMBER_OF_STEPPERS] = {
		&stepper1,
		&stepper2,
		&stepper3,
		&stepper4
	};


Link link1(stepper1, stepper2, stepper3, stepper4, limitBarrier1, limitBarrier2, limitBarrier3, limitBarrier4);
//Link link2(stepper5, stepper6, stepper7, stepper8, limitBarrier5, limitBarrier6, limitBarrier7, limitBarrier8);
//Link link3(stepper9, stepper10, stepper11, stepper12, limitBarrier9, limitBarrier10, limitBarrier11, limitBarrier12);
//Link link4(stepper13, stepper14, stepper15, stepper16, limitBarrier13, limitBarrier14, limitBarrier15, limitBarrier16);

Link *links[NUMBER_OF_LINKS] = {
		&link1
	};


/* Variables */
boolean haveReachedBarriers = false; // Indicates whether the steppers have reached the barriers for the initialization
boolean areCentered = false; // Indicates whether the steppers have reached the center for the initialization
boolean isInitialized = false; // Indicates whether the initialization routine is finished

uint8_t selectedLinkNumber = 1; // Indicates which link is currently selected


/* Method definitions */
void initComponents();
boolean haveReachedBarriersForInit();
void setMovementsToBarrierForInit();
boolean areCenteredForInit();
void setMovementsToCenterForInit();
boolean areInMovement();
void setJoystickMovements();
void update();


/* Methods */
/**
 * \brief Centers all steppers for a consistent start position for all steppers.
 */
void initComponents()
{
	if(haveReachedBarriers == false)
	{
		if(haveReachedBarriersForInit())
		{
			haveReachedBarriers = true;
		}
		else
		{
			setMovementsToBarrierForInit();
		}
	}
	else if(areCentered == false)
	{
		if(areCenteredForInit())
		{
			areCentered = true;
		}
		else
		{
			setMovementsToCenterForInit();
		}
	}
	else
	{
		isInitialized = true;
	}
}


/**
 * \brief Indicates whether all steppers have reached the limit barriers.
 * \return true = all steppers have reached their limit barrier
 */
boolean haveReachedBarriersForInit()
{
	for(uint8_t i = 0; i < NUMBER_OF_LINKS; i++)
	{
		if(links[i]->haveReachedLimitBarriersForInit() == false)
		{
			return false;
		}
	}

	return true;
}


/**
 * \brief Initializes all steppers for the next movement towards the limit barriers.
 */
void setMovementsToBarrierForInit()
{
	for(uint8_t i = 0; i < NUMBER_OF_LINKS; i++)
	{
		links[i]->setMovementsToLimitBarrierForInit();
	}
}


/**
 * \brief Indicates whether all steppers are centered.
 * \return true = all steppers have reached the center
 */
boolean areCenteredForInit()
{
	for(uint8_t i = 0; i < NUMBER_OF_LINKS; i++)
	{
		if(links[i]->isCenteredForInit() == false)
		{
			return false;
		}
	}

	return true;
}


/**
 * \brief Initializes all steppers for the next movement towards the center.
 */
void setMovementsToCenterForInit()
{
	for(uint8_t i = 0; i < NUMBER_OF_LINKS; i++)
	{
		links[i]->setMovementsToCenterForInit();
	}
}


/**
 * \brief Indicates whether a link is still in movmenet.
 * \return true = a stepper has not reached the target position yet
 */
boolean areInMovement()
{
	for(uint8_t i = 0; i < NUMBER_OF_LINKS; i++)
	{
		if(links[i]->isMoving())
		{
			return true;
		}
	}

	return false;
}


/**
 * \brief Set the next link movements, depending on the current joystick state.
 */
void setJoystickMovements()
{
	joystick.read();

	for(uint8_t i = 0; i < NUMBER_OF_LINKS; i++)
	{
		links[i]->setHorizontalDirectionMovement(joystick.getCurrentHorizontalDirection());
		links[i]->setVerticalDirectionMovement(joystick.getCurrentVerticalDirection());
	}
}


/**
 * \brief Updates all links and lets all steppers move which didn't reach the target position.
 */
void update()
{
	for(uint8_t i = 0; i < NUMBER_OF_LINKS; i++)
	{
		links[i]->update();
	}
}


void setup()
{
	Serial.begin(9600);

	while(!isInitialized)
	{
		if(areInMovement())
		{
			update();
			//Serial.println(stepper1.getCurrentPosition());
		}
		else
		{
			initComponents();
		}
	}
}


void loop()
{
	if(areInMovement())
	{
		update();
		//Serial.println(stepper1.getCurrentPosition());
	}
	else
	{
		setJoystickMovements();
	}
}
