/***********************************************************************
 *
 *  Module: DCMotor
 *
 *  File Name: dc_motor.h
 *
 *  Description: Header file for DC Motor module.
 *
 *  Author: Mohamed Khaled
 *
***********************************************************************/

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_


/***********************************************************************
*                       Include common modules                         *
***********************************************************************/
#include "std_types.h"


/***********************************************************************
*                             Definitions                              *
***********************************************************************/
#define	DCMOTOR_M1_PORT_ID 		 PORTB_ID
#define	DCMOTOR_M1_PIN1_ID		 PIN2_ID
#define	DCMOTOR_M1_PIN2_ID		 PIN3_ID


/***********************************************************************
*                           User defined Types                         *
***********************************************************************/
/*
 * The required DC Motor state, it should be CW or A-CW or stop
 */
typedef enum
{
	CW, A_CW, STOP
} DcMotor_State;


/***********************************************************************
*                      Public Functions Prototypes                     *
***********************************************************************/
/*
 * The Function responsible for setup the direction for the two
 * motor pins through the GPIO driver.
 * Stop at the DC-Motor at the beginning through the GPIO driver.
 */
void DcMotor_Init(void);

/*
 * The function responsible for rotate the DC Motor CW/ or A-CW or
 * stop the motor based on the state input state value.
 */
void DcMotor_Rotate(DcMotor_State state);




#endif /* DC_MOTOR_H_ */
