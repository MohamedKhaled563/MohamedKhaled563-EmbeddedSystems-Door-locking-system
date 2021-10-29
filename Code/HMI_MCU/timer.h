/***********************************************************************
 *
 *  Module: TIMER
 *
 *  File Name: timer.h
 *
 *  Description: Header file for AVR TIMER module.
 *
 *  Author: Mohamed Khaled
 *
***********************************************************************/


#ifndef TIMER_H_
#define TIMER_H_


/***********************************************************************
*                       Include common modules                         *
***********************************************************************/
#include "std_types.h"



/***********************************************************************
*                             Definitions                              *
***********************************************************************/



/***********************************************************************
*                           User defined Types                         *
***********************************************************************/
/*
 * Description:
 * To select the required timer.
 */
typedef enum
{
	TIMER0_ID, TIMER1_ID, TIMER2_ID
} TIMER_ID;
/*
 * Description:
 * To select the mode of timer.
 */
typedef enum
{
	OVERFLOW_MODE, COMPARE_MODE
} TIMER_Mode;
/*
 * Description:
 * Select the required prescaler that is used to divide F_CPU.
 */
typedef enum
{
	NO_PRESCALER = 1, F_CPU_8, F_CPU_64, F_CPU_256, F_CPU_1024
} TIMER_Prescaler;
/*
 * Description:
 * Configuration structure for timer module.
 * timer id, required mode of operation, initial value to be loaded before timer start,
 * compare value that is used only in compare mode and prescaler value.
 * All timers operates in Interrupt mode.
 */
typedef struct
{
	TIMER_ID timerID;
	TIMER_Mode mode;
	uint16 initialValue;
	uint16 compareValue;
	TIMER_Prescaler prescaler;
} TIMER_ConfigType;


/***********************************************************************
*                global public variables and functions                 *
***********************************************************************/


/***********************************************************************
*                      Public Functions Prototypes                     *
***********************************************************************/
/*
 * Description:
 * Initialize timer 0, 1 or 2 with specified configurations:
 */
void TIMER_Init( TIMER_ConfigType* config );


/*
 * Description:
 * Function to set the Call Back function address.
 */
void TIMER_setCallBack( void(*a_ptr)( void ), TIMER_ID timerID);


/*
 * Description:
 * Stops the timer by stopping its clock.
 */
void TIMER_Deinit( TIMER_ID timerID );


/***********************************************************************
*                                 Macros                               *
***********************************************************************/



#endif /* TIMER_H_ */
