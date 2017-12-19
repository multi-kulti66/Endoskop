#include "Arduino.h"
#include "Joystick.h"
#include "Button.h"
#include "AccelStepper.h"
#include "Stepper.h"
#include "LimitBarrier.h"
#include "Link.h"

/* Constants */
const uint8_t NUMBER_OF_LINKS = 4;
const uint8_t NUMBER_OF_STEPPERS = NUMBER_OF_LINKS * 4;


/* Components */
Joystick joystick(A8, A9);

Button button1(A3);
Button button2(A2);
Button button3(A1);
Button button4(A0);

Button *buttons[NUMBER_OF_LINKS] = {
	&button1,
	&button2,
	&button3,
	&button4
};

// 1. oben
AccelStepper accelStepper1(AccelStepper::MotorInterfaceType::DRIVER, 28, 26);
Stepper stepper1(accelStepper1);
LimitBarrier limitBarrier1(21);

// 1. rechts
AccelStepper accelStepper2(AccelStepper::MotorInterfaceType::DRIVER, 45, 43);
Stepper stepper2(accelStepper2);
LimitBarrier limitBarrier2(20);

// 1. unten
AccelStepper accelStepper3(AccelStepper::MotorInterfaceType::DRIVER, 33, 31);
Stepper stepper3(accelStepper3);
LimitBarrier limitBarrier3(19);

// 1. links
AccelStepper accelStepper4(AccelStepper::MotorInterfaceType::DRIVER, 37, 35);
Stepper stepper4(accelStepper4);
LimitBarrier limitBarrier4(18);

// 2. oben
AccelStepper accelStepper5(AccelStepper::MotorInterfaceType::DRIVER, 41, 39);
Stepper stepper5(accelStepper5);
LimitBarrier limitBarrier5(17);

// 2. rechts
AccelStepper accelStepper6(AccelStepper::MotorInterfaceType::DRIVER, 32, 30);
Stepper stepper6(accelStepper6);
LimitBarrier limitBarrier6(0);

// 2. unten
AccelStepper accelStepper7(AccelStepper::MotorInterfaceType::DRIVER, 52, 50);
Stepper stepper7(accelStepper7);
LimitBarrier limitBarrier7(15);

// 2. links
AccelStepper accelStepper8(AccelStepper::MotorInterfaceType::DRIVER, 36, 34);
Stepper stepper8(accelStepper8);
LimitBarrier limitBarrier8(14);

// 3. oben
AccelStepper accelStepper9(AccelStepper::MotorInterfaceType::DRIVER, 25, 23);
Stepper stepper9(accelStepper9);
LimitBarrier limitBarrier9(1);

// 3. rechts
AccelStepper accelStepper10(AccelStepper::MotorInterfaceType::DRIVER, 44, 42);
Stepper stepper10(accelStepper10);
LimitBarrier limitBarrier10(16);

// 3. unten
AccelStepper accelStepper11(AccelStepper::MotorInterfaceType::DRIVER, 40, 38);
Stepper stepper11(accelStepper11);
LimitBarrier limitBarrier11(2);

// 3. links
AccelStepper accelStepper12(AccelStepper::MotorInterfaceType::DRIVER, 48, 46);
Stepper stepper12(accelStepper12);
LimitBarrier limitBarrier12(3);

// 4. oben
AccelStepper accelStepper13(AccelStepper::MotorInterfaceType::DRIVER, 49, 47);
Stepper stepper13(accelStepper13);
LimitBarrier limitBarrier13(4);

// 4. rechts
AccelStepper accelStepper14(AccelStepper::MotorInterfaceType::DRIVER, 53, 51);
Stepper stepper14(accelStepper14);
LimitBarrier limitBarrier14(5);

// 4. unten
AccelStepper accelStepper15(AccelStepper::MotorInterfaceType::DRIVER, 29, 27);
Stepper stepper15(accelStepper15);
LimitBarrier limitBarrier15(6);

// 4. links
AccelStepper accelStepper16(AccelStepper::MotorInterfaceType::DRIVER, 24, 22);
Stepper stepper16(accelStepper16);
LimitBarrier limitBarrier16(7);

Stepper *steppers[NUMBER_OF_STEPPERS] = {
	&stepper1,
	&stepper2,
	&stepper3,
	&stepper4,
	&stepper5,
	&stepper6,
	&stepper7,
	&stepper8,
	&stepper9,
	&stepper10,
	&stepper11,
	&stepper12,
	&stepper13,
	&stepper14,
	&stepper15,
	&stepper16
};


Link link1(stepper1, stepper2, stepper3, stepper4, limitBarrier1, limitBarrier2, limitBarrier3, limitBarrier4);
Link link2(stepper5, stepper6, stepper7, stepper8, limitBarrier5, limitBarrier6, limitBarrier7, limitBarrier8);
Link link3(stepper9, stepper10, stepper11, stepper12, limitBarrier9, limitBarrier10, limitBarrier11, limitBarrier12);
Link link4(stepper13, stepper14, stepper15, stepper16, limitBarrier13, limitBarrier14, limitBarrier15, limitBarrier16);

Link *links[NUMBER_OF_LINKS] = {
	&link1,
	&link2,
	&link3,
	&link4,
};


/* Variables */
boolean haveReachedBarriers = false; // Indicates whether the steppers have reached the barriers for the initialization
boolean areCentered = false; // Indicates whether the steppers have reached the center for the initialization
boolean isInitialized = false; // Indicates whether the initialization routine is finished

uint8_t selectedLinkIndex = 0; // Indicates which link is currently selected
uint8_t counter = 0;


/* Method definitions */
void initComponents();
boolean haveReachedBarriersForInit();
void setMovementsToBarrierForInit();
boolean areCenteredForInit();
void setMovementsToCenterForInit();
void getButtonState();
void setMovements();
void update();


/* Methods */
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


void setMovementsToBarrierForInit()
{
	for(uint8_t i = 0; i < NUMBER_OF_LINKS; i++)
	{
		links[i]->setMovementsToLimitBarrierForInit();
	}
}


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


void setMovementsToCenterForInit()
{
	for(uint8_t i = 0; i < NUMBER_OF_LINKS; i++)
	{
		links[i]->setMovementsToCenterForInit();
	}
}

void getButtonState()
{
	for(int i = NUMBER_OF_LINKS - 1; i >= 0; i--)
	{
		if(buttons[i]->isButtonPressed())
		{
			selectedLinkIndex = i;
		}
	}
}


void setMovements()
{
	getButtonState();
	joystick.read();

	links[selectedLinkIndex]->setHorizontalDirectionMovement(joystick.getCurrentHorizontalDirection());
	links[selectedLinkIndex]->setVerticalDirectionMovement(joystick.getCurrentVerticalDirection());
}


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
		initComponents();
		update();
	}
}


void loop()
{
	setMovements();
	update();
}
