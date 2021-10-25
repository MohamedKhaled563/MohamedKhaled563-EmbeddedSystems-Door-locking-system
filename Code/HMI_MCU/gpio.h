/***********************************************************************
 *
 *  Module: GPIO
 *
 *  File Name: gpio.h
 *
 *  Description: Header file for AVR GPIO module.
 *
 *  Author: Mohamed Khaled
 *
***********************************************************************/


#ifndef GPIO_H_
#define GPIO_H_

/***********************************************************************
*                       Include common modules                         *
***********************************************************************/
#include "std_types.h"



/***********************************************************************
*                             Definitions                              *
***********************************************************************/
#define NUMBER_OF_PORTS 4
#define NUMBER_OF_PINS_PER_PORT 7

#define PORTA_ID 0
#define PORTB_ID 1
#define PORTC_ID 2
#define PORTD_ID 3

#define PIN0_ID 0
#define PIN1_ID 1
#define PIN2_ID 2
#define PIN3_ID 3
#define PIN4_ID 4
#define PIN5_ID 5
#define PIN6_ID 6
#define PIN7_ID 7



/***********************************************************************
*                           User defined Types                         *
***********************************************************************/
typedef enum {
	PIN_INPUT, PIN_OUTPUT
} PinDirectionType;

typedef enum {
	PORT_INPUT, PORT_OUTPUT = 0XFF
} PortDirectionType;



/***********************************************************************
*                global public variables and functions                 *
***********************************************************************/


/***********************************************************************
*                      Public Functions Prototypes                     *
***********************************************************************/
/*
 * Description:
 * This function is used to setup pin direction
 * It requires the port number and pin number and the required direction for the pin.
 * If the port number or pin number are not correct, the function will not handle the request.
 */
void GPIO_setupPinDirection(uint8 a_portID, uint8 a_pinID, PinDirectionType a_direction);

/*
 * Description:
 * This function is used to write LOGIC_HIGH or LOGIC_LOW on certain pin.
 * It requires port number and pin number and the required value to be written.
 * If the port number or pin number are not correct, the function will not handle the request.
 */
void GPIO_writePin(uint8 a_portID, uint8 a_pinID, boolean a_value);

/*
 * Description:
 * Read the value of certain pin LOGIC_HIGH or LOGIC_LOW.
 * If the port number or pin number are not correct, the function will not handle the request.
 */
boolean GPIO_readPin(uint8 a_portID, uint8 a_pinID);

/*
 * Description:
 * This function is used to setup port direction
 * It requires the port number and the required direction for the port.
 * If the port number are not correct, the function will not handle the request.
 */
void GPIO_setupPortDirection(uint8 a_portID, PortDirectionType a_direction);

/*
 * Description:
 * This function is used to write 8bits value on certain port.
 * It requires port number and the required value to be written.
 * If the port number is not correct, the function will not handle the request.
 */
void GPIO_writePort(uint8 a_portID, uint8 a_value);

/*
 * Description:
 * Read the 8bits value of certain port.
 * If the port number is not correct, the function will not handle the request.
 */
uint8 GPIO_readPort(uint8 a_portID);



/***********************************************************************
*                                 Macros                               *
***********************************************************************/



#endif /* GPIO_H_ */
