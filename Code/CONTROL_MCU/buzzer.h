/*
 *
 * Module: Buzzer.
 *
 * File Name: buzzer.h
 *
 * Description: Header file of buzzer module.
 *
 * Author: Mohamed Khaled.
 *
 */


#ifndef BUZZER_H_
#define BUZZER_H_

/***********************************************************************
*                       Include common modules                         *
***********************************************************************/
#include "std_types.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define BUZZER_PORT_ID				PORTB_ID
#define BUZZER_PIN_ID				PIN1_ID


/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*
 * Description:
 * Initialize buzzer by specifying its pin as output pin.
 */
void BUZZER_Init( void );


/*
 * Description:
 * Turn on buzzer.
 */
void BUZZER_On( void );

/*
 * Description:
 * Turn off buzzer.
 */
void BUZZER_Off( void );


#endif /* BUZZER_H_ */
