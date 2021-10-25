/*
 *
 * Module: GPIO
 *
 * File Name: gpio.c
 *
 * Description: Source file for AVR GPIO driver.
 *
 * Author: Mohamed Khaled
 *
 */


/***********************************************************************
 *                      Include Module header file                      *
 ***********************************************************************/
#include "gpio.h"



/***********************************************************************
 *              Include the other required header files                 *
 ***********************************************************************/
#include <avr/io.h> /* To use IO registers */
#include "common_macros.h" /* To use macros like SET_BIT */



/***********************************************************************
 *                            Global Variables                          *
 ***********************************************************************/



/***********************************************************************
 *                      Local Functions Prototypes                      *
 ***********************************************************************/




/***********************************************************************
 *                          Functions Definitions                       *
 ***********************************************************************/
/*
 * Description:
 * This function is used to setup pin direction
 * It requires the port number and pin number and the required direction for the pin.
 * If the port number or pin number are not correct, the function will not handle the request.
 */
void GPIO_setupPinDirection(uint8 a_portID, uint8 a_pinID, PinDirectionType a_direction)
{
	/*If the port number or pin number are not correct, the function will not handle the request.*/
	if(a_portID >= NUMBER_OF_PORTS || a_pinID >= NUMBER_OF_PINS_PER_PORT)
	{
		return;
	}
	else
	{
		/*
		 * Select the correct register based on port
		 * If required direction is input, clear bit in the port DDR register
		 * If required direction is output, set bit in the port DDR register
		 */
		switch(a_portID)
		{
		case PORTA_ID:
			if(a_direction == PIN_INPUT)
			{
				CLEAR_BIT(DDRA, a_pinID);
			}
			else
			{
				SET_BIT(DDRA, a_pinID);
			}
			break;
		case PORTB_ID:
			if(a_direction == PIN_INPUT)
			{
				CLEAR_BIT(DDRB, a_pinID);
			}
			else
			{
				SET_BIT(DDRB, a_pinID);
			}
			break;
		case PORTC_ID:
			if(a_direction == PIN_INPUT)
			{
				CLEAR_BIT(DDRC, a_pinID);
			}
			else
			{
				SET_BIT(DDRC, a_pinID);
			}
			break;
		case PORTD_ID:
			if(a_direction == PIN_INPUT)
			{
				CLEAR_BIT(DDRD, a_pinID);
			}
			else
			{
				SET_BIT(DDRD, a_pinID);
			}
			break;
		}
	}
}

/*
 * Description:
 * This function is used to write LOGIC_HIGH or LOGIC_LOW on certain pin.
 * It requires port number and pin number and the required value to be written.
 * If the port number or pin number are not correct, the function will not handle the request.
 */
void GPIO_writePin(uint8 a_portID, uint8 a_pinID, boolean a_value)
{
	/*If the port number or pin number are not correct, the function will not handle the request.*/
	if(a_portID >= NUMBER_OF_PORTS || a_pinID >= NUMBER_OF_PINS_PER_PORT)
	{
		return;
	}
	else
	{
		/*
		 * Select the correct register based on port
		 * If required value is FALSE, clear bit in the port PORT register
		 * If required value is TRUE, set bit in the port PORT register
		 */
		switch(a_portID)
		{
		case PORTA_ID:
			if(a_value == LOGIC_HIGH)
			{
				SET_BIT(PORTA, a_pinID);
			}
			else
			{
				CLEAR_BIT(PORTA, a_pinID);
			}
			break;
		case PORTB_ID:
			if(a_value == LOGIC_HIGH)
			{
				SET_BIT(PORTB, a_pinID);
			}
			else
			{
				CLEAR_BIT(PORTB, a_pinID);
			}
			break;
		case PORTC_ID:
			if(a_value == LOGIC_HIGH)
			{
				SET_BIT(PORTC, a_pinID);
			}
			else
			{
				CLEAR_BIT(PORTC, a_pinID);
			}
			break;
		case PORTD_ID:
			if(a_value == LOGIC_HIGH)
			{
				SET_BIT(PORTD, a_pinID);
			}
			else
			{
				CLEAR_BIT(PORTD, a_pinID);
			}
			break;
		}
	}
}

/*
 * Description:
 * Read the value of certain pin LOGIC_HIGH or LOGIC_LOW.
 * If the port number or pin number are not correct, the function will not handle the request.
 */
boolean GPIO_readPin(uint8 a_portID, uint8 a_pinID)
{
	/*If the port number or pin number are not correct, the function will not handle the request.*/
	if(a_portID >= NUMBER_OF_PORTS || a_pinID >= NUMBER_OF_PINS_PER_PORT)
	{
		return LOGIC_LOW;
	}
	else
	{
		/* Select the correct PIN register according to the port id. */
		boolean result = LOGIC_LOW;
		switch(a_portID)
		{
		case PORTA_ID:
			(BIT_IS_SET(PINA, a_pinID))? (result = LOGIC_HIGH): (result = LOGIC_LOW);
			break;
		case PORTB_ID:
			(BIT_IS_SET(PINB, a_pinID))? (result = LOGIC_HIGH): (result = LOGIC_LOW);
			break;
		case PORTC_ID:
			(BIT_IS_SET(PINC, a_pinID))? (result = LOGIC_HIGH): (result = LOGIC_LOW);
			break;
		case PORTD_ID:
			(BIT_IS_SET(PIND, a_pinID))? (result = LOGIC_HIGH): (result = LOGIC_LOW);
			break;
		}
		return result;
	}
}

/*
 * Description:
 * This function is used to setup port direction
 * It requires the port number and the required direction for the port.
 * If the port number are not correct, the function will not handle the request.
 */
void GPIO_setupPortDirection(uint8 a_portID, PortDirectionType a_direction)
{
	/* Check that port id is valid */
	if(a_portID >= NUMBER_OF_PORTS)
	{
		return;
	}
	else
	{
		/* Select the correct PORT register according to sent port id */
		switch(a_portID)
		{
		case PORTA_ID:
			DDRA = a_direction;
			break;
		case PORTB_ID:
			DDRB = a_direction;
			break;
		case PORTC_ID:
			DDRC = a_direction;
			break;
		case PORTD_ID:
			DDRD = a_direction;
			break;
		}
	}
}

/*
 * Description:
 * This function is used to write 8bits value on certain port.
 * It requires port number and the required value to be written.
 * If the port number is not correct, the function will not handle the request.
 */
void GPIO_writePort(uint8 a_portID, uint8 a_value)
{
	/* Check that port id is valid */
	if(a_portID >= NUMBER_OF_PORTS)
	{
		return;
	}
	else
	{
		/* Select the correct PORT register according to sent port id */
		switch(a_portID)
		{
		case PORTA_ID:
			PORTA = a_value;
			break;
		case PORTB_ID:
			PORTB = a_value;
			break;
		case PORTC_ID:
			PORTC = a_value;
			break;
		case PORTD_ID:
			PORTD = a_value;
			break;
		}
	}
}

/*
 * Description:
 * Read the 8bits value of certain port.
 * If the port number is not correct, the function will not handle the request.
 */
uint8 GPIO_readPort(uint8 a_portID)
{
	/* Check that port id is valid */
	if(a_portID >= NUMBER_OF_PORTS)
	{
		return 0;
	}
	else
	{
		/* Select the correct PIN register according to sent port id */
		switch(a_portID)
		{
		case PORTA_ID:
			return PINA;
			break;
		case PORTB_ID:
			return PINB;
			break;
		case PORTC_ID:
			return PINC;
			break;
		case PORTD_ID:
			return PIND;
			break;
		default:
			return 0;
		}
	}
}

/***********************************************************************
 *                              ISRs code                               *
 ***********************************************************************/
