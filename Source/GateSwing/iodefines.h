/*
 * defines.h
 *
 * Created: 18.03.2016 19:03:31
 *  Author: HSO
 */ 


#ifndef DEFINES_H_
#define DEFINES_H_

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/delay.h>
#include "libs/utils/bit_tools.h"


/************************************************************************/
/* DEFINE SIGNALS INPUT                                                                     */
/************************************************************************/
#define SIGNAL_REMOTE_PED 1
#define SIGNAL_REMOTE_SWING 2

#define BUTTON_ESC 3
#define BUTTON_ENTER 4
#define BUTTON_UP 5
#define BUTTON_DOWN 6
#define BUTTON_PRG 7 // ENTER + ESC


#define SIGNAL_PHOTOCELL_BLOCKED 3
#define SIGNAL_FCA_M1 4
#define SIGNAL_FCC_M1 5
#define SIGNAL_FCA_M2 6
#define SIGNAL_FCC_M2 7

/************************************************************************/
/* @define relay output ports                                                                     */
/************************************************************************/
#define RELAY_PORT PORTF

#define PIN_MOTOR_M1_OPEN 0
#define PIN_MOTOR_M1_CLOSE 1

#define PIN_MOTOR_M2_OPEN 2
#define PIN_MOTOR_M2_CLOSE 3

#define PIN_LAMP 4
#define PIN_LOCK 5
#define PIN_LIGHT 6
#define PIN_POWER_CELLS 7

/************************************************************************/
/* VARIABLES state                                                                     */
/************************************************************************/
#define ACTIVE 1
#define INACTIVE 0

#define CELL_FREE 0
#define REED_ACTIVE 0

/************************************************************************/
/* @ set relays states                                                                     */
/************************************************************************/
#define RELAY_DELAY 100
#define OPEN_M1 do{bit_clear(RELAY_PORT,PIN_MOTOR_M1_CLOSE); _delay_ms(RELAY_DELAY); bit_set(RELAY_PORT,PIN_MOTOR_M1_OPEN);}while(0)
#define CLOSE_M1 do{bit_clear(RELAY_PORT,PIN_MOTOR_M1_OPEN); _delay_ms(RELAY_DELAY); bit_set(RELAY_PORT,PIN_MOTOR_M1_CLOSE);}while(0)
#define IDLE_M1 do{bit_clear(RELAY_PORT,PIN_MOTOR_M1_CLOSE); bit_clear(RELAY_PORT,PIN_MOTOR_M1_OPEN);}while(0)

#define OPEN_M2 do{bit_clear(RELAY_PORT,PIN_MOTOR_M2_CLOSE); _delay_ms(RELAY_DELAY); bit_set(RELAY_PORT,PIN_MOTOR_M2_OPEN);}while(0)
#define CLOSE_M2 do{bit_clear(RELAY_PORT,PIN_MOTOR_M2_OPEN); _delay_ms(RELAY_DELAY); bit_set(RELAY_PORT,PIN_MOTOR_M2_CLOSE);}while(0)
#define IDLE_M2 do{bit_clear(RELAY_PORT,PIN_MOTOR_M2_CLOSE); bit_clear(RELAY_PORT,PIN_MOTOR_M2_OPEN);}while(0)

#define SET_UNLOCK bit_set(RELAY_PORT,PIN_LOCK)
#define IDLE_UNLOCK bit_clear(RELAY_PORT,PIN_LOCK)

#define SET_POWER_CELLS bit_set(RELAY_PORT,PIN_POWER_CELLS)
#define IDLE_POWER_CELLS bit_clear(RELAY_PORT,PIN_POWER_CELLS)

#define SET_LIGHTS bit_set(RELAY_PORT,PIN_LIGHT)
#define IDLE_LIGHTS bit_clear(RELAY_PORT,PIN_LIGHT)

#define SET_LAMP bit_set(RELAY_PORT,PIN_LAMP)
#define IDLE_LAMP bit_clear(RELAY_PORT,PIN_LAMP)




#endif /* DEFINES_H_ */