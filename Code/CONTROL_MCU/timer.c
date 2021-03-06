/*
 *
 * Module: TIMER
 *
 * File Name: timer.c
 *
 * Description: Source file for AVR TIMER driver.
 *
 * Author: Mohamed Khaled
 *
 */
/***********************************************************************
 *                      Include Module header file                      *
 ***********************************************************************/
#include "timer.h"


/***********************************************************************
 *              Include the other required header files                 *
 ***********************************************************************/
#include <avr/io.h> /* To use IO registers */
#include <avr/interrupt.h>


/***********************************************************************
 *                            Global Variables                          *
 ***********************************************************************/
/*
 * Description:
 * Global variables to hold the address of the call back function of timer0
 * that can be used in any mode of timer0 (overflow or compare)
 */
static volatile void (*g_timer0CallBackPtr)(void) = NULL_PTR;
/*
 * Description:
 * Global variables to hold the address of the call back function of timer0
 * that can be used in any mode of timer1 (overflow or compare)
 */
static volatile void (*g_timer1CallBackPtr)(void) = NULL_PTR;
/*
 * Description:
 * Global variables to hold the address of the call back function of timer0
 * that can be used in any mode of timer2 (overflow or compare)
 */
static volatile void (*g_timer2CallBackPtr)(void) = NULL_PTR;


/***********************************************************************
 *                          Functions Definitions                       *
 ***********************************************************************/
/*
 * Description:
 * Initialize timer 0, 1 or 2 with specified configurations:
 */
void TIMER_Init( TIMER_ConfigType* config )
{
	/* Select the required timer. */
	switch(config->timerID)
	{
	/***********************************TIMER0***********************************/
	case TIMER0_ID:
		/* Put the required initial value of timer in the timer0 8bits counter register.*/
		TCNT0 = (uint8)config->initialValue;
		/*
		 * ********* TCCR0 Register bits description *********
		 * FOC0 = 1 Non PWM mode.
		 * WGM01:0 = 00 if Normal(overflow) mode and = 10 in Compare mode.
		 * COM01:0 = 00 if Normal(overflow) mode and = 00 in Compare mode.
		 * CS02:0 = 000 if no clock source and a value = TIMER_Prescaler if there is a clock.
		 */
		TCCR0 = (1<<FOC0);
		/* Inserting clock in the first three bits in TCCR0. */
		TCCR0 = (TCCR0&~(0x07)) | (config->prescaler);
		switch(config->mode)
		{
		case OVERFLOW_MODE:
			/* Enable overflow interrupt for required mode. */
			TIMSK |= (1<<TOIE0);
			break;
			/* Set bits that need to be set in compare mode, otherwise let them 0s. */
		case COMPARE_MODE:
			TCCR0 |= (1<<WGM01);
			/* Store compare value in case of mode is COMPARE_MODE. */
			OCR0 = config->compareValue;
			/* Enable compare match interrupt for required mode. */
			TIMSK |= (1<<OCIE0);
			break;
		}
		break;
		/***********************************TIMER1***********************************/
		case TIMER1_ID:
			/* Put the required initial value of timer in the timer1 16 bits counter register.*/
			TCNT1 = config->initialValue;
			/*
			 * ********* TCCR1A Register bits description *********
			 * FOC1A,FOC1B = 1 Non PWM mode.
			 * WGM01:0 = 00 if Normal(overflow) mode and = 10 in Compare mode.
			 * COM1A1:0 = 00 if Normal(overflow) mode and = 00 in Compare mode.
			 * CS02:0 = 000 if no clock source and a value = TIMER_Prescaler if there is a clock.
			 */
			TCCR1A = (1<<FOC1A) | (1<<FOC1B);
			/* Inserting clock in the first three bits in TCCR0. */
			TCCR1B = (TCCR1B&~(0x07)) | (config->prescaler);
			switch(config->mode)
			{
			case OVERFLOW_MODE:
				/* Enable overflow interrupt for required mode. */
				TIMSK |= (1<<TOIE1);
				break;
				/* Set bits that need to be set in compare mode, otherwise let them 0s. */
			case COMPARE_MODE:
				TCCR1B |= (1<<WGM12);
				/* Store compare value in case of mode is COMPARE_MODE. */
				OCR1A = config->compareValue;
				/* Enable compare match interrupt for required mode. */
				TIMSK |= (1<<OCIE1A);
				break;
			}
			break;
			/***********************************TIMER2***********************************/
			case TIMER2_ID:
				/* Put the required initial value of timer in the timer2 8bits counter register.*/
				TCNT2 = (uint8)config->initialValue;
				/*
				 * ********* TCCR2 Register bits description *********
				 * FOC2 = 1 Non PWM mode.
				 * WGM21:0 = 00 if Normal(overflow) mode and = 10 in Compare mode.
				 * COM01:0 = 00 if Normal(overflow) mode and = 00 in Compare mode.
				 * CS22:0 = 000 if no clock source and a value = TIMER_Prescaler if there is a clock.
				 */
				TCCR2 = (1<<FOC2);
				/* Inserting clock in the first three bits in TCCR0. */
				TCCR2 = (TCCR2&~(0x07)) | (config->prescaler);
				switch(config->mode)
				{
				case OVERFLOW_MODE:
					/* Enable overflow interrupt for required mode. */
					TIMSK |= (1<<TOIE2);
					break;
					/* Set bits that need to be set in compare mode, otherwise let them 0s. */
				case COMPARE_MODE:
					TCCR2 |= (1<<WGM21);
					/* Store compare value in case of mode is COMPARE_MODE. */
					OCR2 = config->compareValue;
					/* Enable compare match interrupt for required mode. */
					TIMSK |= (1<<OCIE2);
					break;
				}
	}
}


/*
 * Description:
 * Function to set the Call Back function address.
 */
void TIMER_setCallBack( void(*a_ptr)( void ), TIMER_ID timerID)
{
	/* Save the address of the Call back function in a global variable */
	switch(timerID)
	{
	case TIMER0_ID:
		g_timer0CallBackPtr = a_ptr;
		break;
	case TIMER1_ID:
		g_timer1CallBackPtr = a_ptr;
		break;
	case TIMER2_ID:
		g_timer2CallBackPtr = a_ptr;
		break;
	}
}


/*
 * Description:
 * Stops the timer by stopping its clock.
 */
void TIMER_Deinit( TIMER_ID timerID )
{
	switch(timerID)
	{
	case TIMER0_ID:
		TCCR0 = 0;
		break;
	case TIMER1_ID:
		TCCR1B = 0;
		break;
	case TIMER2_ID:
		TCCR2 = 0;
		break;
	}
}


/***********************************************************************
 *                              ISRs code                               *
 ***********************************************************************/
/*
 * Description:
 * Calls the call back function of timer 0 in case of Overflow interrupt occurs.
 */
ISR( TIMER0_OVF_vect )
{
	if(g_timer0CallBackPtr != NULL_PTR)
	{
		(*g_timer0CallBackPtr)();
	}
	TIFR |= (1<<TOV0);
}
/*
 * Description:
 * Calls the call back function of timer 0 in case of compare match interrupt occurs.
 */
ISR( TIMER0_COMP_vect )
{
	if(g_timer0CallBackPtr != NULL_PTR)
	{
		(*g_timer0CallBackPtr)();
	}
	TIFR |= (1<<OCF0);
}
/*
 * Description:
 * Calls the call back function of timer 1 in case of Overflow interrupt occurs.
 */
ISR( TIMER1_OVF_vect )
{
	if(g_timer1CallBackPtr != NULL_PTR)
	{
		(*g_timer1CallBackPtr)();
	}
	TIFR |= (1<<TOV1);
}
/*
 * Description:
 * Calls the call back function of timer 1 in case of compare match interrupt occurs.
 */
ISR( TIMER1_COMPA_vect )
{
	if(g_timer1CallBackPtr != NULL_PTR)
	{
		(*g_timer1CallBackPtr)();
	}
	TIFR |= (1<<OCF1A);
}
/*
 * Description:
 * Calls the call back function of timer 2 in case of Overflow interrupt occurs.
 */
ISR( TIMER2_OVF_vect )
{
	if(g_timer2CallBackPtr != NULL_PTR)
	{
		(*g_timer2CallBackPtr)();
	}
	TIFR |= (1<<TOV2);
}
/*
 * Description:
 * Calls the call back function of timer 2 in case of compare match interrupt occurs.
 */
ISR( TIMER2_COMP_vect )
{
	if(g_timer2CallBackPtr != NULL_PTR)
	{
		(*g_timer2CallBackPtr)();
	}
	TIFR |= (1<<OCF2);
}
