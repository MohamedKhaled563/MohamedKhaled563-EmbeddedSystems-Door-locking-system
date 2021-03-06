/*
 *
 * Module: DELAY
 *
 * File Name: delay.c
 *
 * Description: Source file for delay module.
 *
 * Author: Mohamed Khaled
 *
 */


/***********************************************************************
 *                      Include Module header file                      *
 ***********************************************************************/
#include "delay.h"


/***********************************************************************
 *              Include the other required header files                 *
 ***********************************************************************/
#include "timer.h"


/***********************************************************************
 *                            Global Variables                          *
 ***********************************************************************/
static uint32 g_timer1Tick = 0;


/***********************************************************************
 *                      Local Functions Prototypes                      *
 ***********************************************************************/
/*
 * Description:
 * This function used as callback function for timer1 to help making the delay.
 * This function will be called each when the Compare or overflow interrupt of TIMER1 occurs.
 */
static void timer1Tick( void );


/***********************************************************************
 *                          Functions Definitions                       *
 ***********************************************************************/
/*
 * Description:
 * This function used as callback function for timer1 to help making the delay.
 * This function will be called each when the Compare or overflow interrupt of TIMER1 occurs.
 */
static void timer1Tick( void )
{
	g_timer1Tick++;
}


/*
 * Description:
 * Make busy waiting delay by n ms.
 * This function uses Timer 1 in compare match mode.
 */
void delay_ms( uint32 n )
{
	g_timer1Tick = 0;
	/*
	 * Configuration structure for timer.
	 * timer id, required mode of operation, initial value to be loaded before timer start,
	 * compare value that is used only in compare mode and prescaler value.
	 */
	/* T_TIMER1 = 1us, to get 1ms, Put compare value = 1000 to get 1ms. */
	TIMER_ConfigType config = {TIMER1_ID, COMPARE_MODE, 0, 1000, F_CPU_8};
	TIMER_Init(&config);
	TIMER_setCallBack(timer1Tick, TIMER1_ID);
	/* Busy wait for n ms. */
	while( g_timer1Tick< n );
	TIMER_Deinit(TIMER1_ID);
}

