/***********************************************************************
 *
 *  Module: DCMotor
 *
 *  File Name: dc_motor.c
 *
 *  Description: Source file for DC Motor module.
 *
 *  Author: Mohamed Khaled
 *
***********************************************************************/

/***********************************************************************
 *                      Include Module header file                      *
 ***********************************************************************/
#include "dc_motor.h"


/***********************************************************************
 *              Include the other required header files                 *
 ***********************************************************************/
#include "gpio.h"


/***********************************************************************
 *                          Functions Definitions                       *
 ***********************************************************************/
/*
 * The Function responsible for setup the direction for the two
 * motor pins through the GPIO driver.
 * Stop at the DC-Motor at the beginning through the GPIO driver.
 */
void DcMotor_Init(void)
{
	/* Setup motor control pins as output */
	GPIO_setupPinDirection(DCMOTOR_M1_PORT_ID, DCMOTOR_M1_PIN1_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(DCMOTOR_M1_PORT_ID, DCMOTOR_M1_PIN2_ID, PIN_OUTPUT);
	/* Initially stop motor */
	GPIO_writePin(DCMOTOR_M1_PORT_ID, DCMOTOR_M1_PIN1_ID, LOGIC_LOW);
	GPIO_writePin(DCMOTOR_M1_PORT_ID, DCMOTOR_M1_PIN2_ID, LOGIC_LOW);
}


/*
 * The function responsible for rotate the DC Motor CW/ or A-CW or
 * stop the motor based on the state input state value.
 */
void DcMotor_Rotate(DcMotor_State state)
{
	switch(state)
	{
	case CW:
		GPIO_writePin(DCMOTOR_M1_PORT_ID, DCMOTOR_M1_PIN1_ID, LOGIC_LOW);
		GPIO_writePin(DCMOTOR_M1_PORT_ID, DCMOTOR_M1_PIN2_ID, LOGIC_HIGH);
		break;
	case A_CW:
		GPIO_writePin(DCMOTOR_M1_PORT_ID, DCMOTOR_M1_PIN1_ID, LOGIC_HIGH);
		GPIO_writePin(DCMOTOR_M1_PORT_ID, DCMOTOR_M1_PIN2_ID, LOGIC_LOW);
		break;
	case STOP:
		GPIO_writePin(DCMOTOR_M1_PORT_ID, DCMOTOR_M1_PIN1_ID, LOGIC_LOW);
		GPIO_writePin(DCMOTOR_M1_PORT_ID, DCMOTOR_M1_PIN2_ID, LOGIC_LOW);
		break;
	}
}







