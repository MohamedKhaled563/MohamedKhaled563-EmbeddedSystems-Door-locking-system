/*
 *
 * Module: Buzzer.
 *
 * File Name: buzzer.c
 *
 * Description: Source file of buzzer module.
 *
 * Author: Mohamed Khaled.
 *
 */

/***********************************************************************
 *                      Include Module header file                      *
 ***********************************************************************/
#include "buzzer.h"


/***********************************************************************
 *              Include the other required header files                 *
 ***********************************************************************/
#include "gpio.h"


/***********************************************************************
 *                          Functions Definitions                       *
 ***********************************************************************/
/*
 * Description:
 * Initialize buzzer by specifying its pin as output pin.
 */
void BUZZER_Init( void )
{
	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);
}


/*
 * Description:
 * Turn on buzzer.
 */
void BUZZER_On( void )
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_HIGH);
}


/*
 * Description:
 * Turn off buzzer.
 */
void BUZZER_Off( void )
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}

