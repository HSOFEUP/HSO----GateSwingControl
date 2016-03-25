/*
 * lcdmessages.c
 *
 * Created: 16.03.2016 18:35:06
 *  Author: HSO
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <stdio.h>
#include <avr/delay.h>
#include "libs/lcds/hd774x.h"
#include "statemachine.h"
#include "iodefines.h"
#include "power.h"



/************************************************************************/
/* Globals Extern                                                                     */
/************************************************************************/
extern volatile uint8_t stateProgram;

extern volatile uint8_t preFlashingActive;
extern volatile uint8_t ramBlowActive;
extern volatile uint8_t stepActive;
extern volatile uint8_t cellActive;
extern volatile uint8_t autoCloseActive;

extern volatile uint16_t powerInitial;
extern volatile uint16_t powerCourse;
extern volatile uint16_t preFlashingTime;
extern volatile uint16_t ramBlowTime;
extern volatile uint16_t delayGatesTime; // seconds
extern volatile uint16_t workingTime;
extern volatile uint16_t waitingTime;
extern volatile uint16_t swingGatesTime;
extern volatile uint16_t unlockHoldTime;
extern volatile uint16_t holdingTime;



/************************************************************************/
/* @ show lcd prog messages                                                                     */
/************************************************************************/
void lcdShowProgram(uint8_t state){
	char tempBuffer[20];
	
	switch(state){
		case PRG_STATE_RUN:
		LCD_gotoXY(0,0);
		//////////////////////////////////
		LCD_sendString("MODE: RUN        ");
		
		LCD_gotoXY(0,1);
		LCD_sendString("                   ");
		break;
		
		
		case PRG_STATE_SETUP_POWER_INI:
		LCD_gotoXY(0,0);
		//////////////////////////////////
		LCD_sendString("MODE: POWER INI  ");
		//////////////////////////////////
		sprintf(tempBuffer,"POWER     %4d",powerInitial);
		
		LCD_gotoXY(0,1);
		LCD_sendString(tempBuffer);
		
		break;
		
		
		case PRG_STATE_SETUP_POWER_COU:
		LCD_gotoXY(0,0);
		//////////////////////////////////
		LCD_sendString("MODE: POWER COU  ");
		//////////////////////////////////
		sprintf(tempBuffer,"POWER     %4d",powerCourse);
		
		LCD_gotoXY(0,1);
		LCD_sendString(tempBuffer);
		
		break;
		
		
		
		case PRG_STATE_SETUP_PREFLASHING_MODE:
		LCD_gotoXY(0,0);
		//////////////////////////////////
		LCD_sendString("MODE: FLASH MODE ");
		
		if(preFlashingActive == ACTIVE){
			//////////////////////////////////
			sprintf(tempBuffer,"ACTIVE             ");
			}else{
			//////////////////////////////////
			sprintf(tempBuffer,"INACTIVE           ");
		}
		LCD_gotoXY(0,1);
		LCD_sendString(tempBuffer);
		
		break;
		
		case PRG_STATE_SETUP_PREFLASHING_TIME:
		LCD_gotoXY(0,0);
		//////////////////////////////////
		LCD_sendString("MODE: FLASH TIME ");
		
		if(preFlashingActive == ACTIVE){
			//////////////////////////////////
			sprintf(tempBuffer,"ACTIVE    %4d",preFlashingTime);
			}else{
			//////////////////////////////////
			sprintf(tempBuffer,"INACTIVE  %4d",preFlashingTime);
		}
		LCD_gotoXY(0,1);
		LCD_sendString(tempBuffer);
		
		break;
		
		
		case PRG_STATE_SETUP_UNLOCK_TIME:
		LCD_gotoXY(0,0);
		//////////////////////////////////
		LCD_sendString("MODE: UNLOCK TIME");
		//////////////////////////////////
		sprintf(tempBuffer,"UNLOCK    %4d",unlockHoldTime);
		LCD_gotoXY(0,1);
		LCD_sendString(tempBuffer);
		
		break;
		
		case PRG_STATE_SETUP_RAMBLOW_MODE:
		LCD_gotoXY(0,0);
		//////////////////////////////////
		LCD_sendString("MODE: RAM MODE   ");
		
		if(ramBlowActive == ACTIVE){
			//////////////////////////////////
			sprintf(tempBuffer,"ACTIVE             ");
			}else{
			//////////////////////////////////
			sprintf(tempBuffer,"INACTIVE           ");
		}
		LCD_gotoXY(0,1);
		LCD_sendString(tempBuffer);
		
		break;
		
		
		case PRG_STATE_SETUP_RAMBLOW_TIME:
		LCD_gotoXY(0,0);
		//////////////////////////////////
		LCD_sendString("MODE: RAM TIME   ");
		
		if(ramBlowActive == ACTIVE){
			//////////////////////////////////
			sprintf(tempBuffer,"ACTIVE    %4d",ramBlowTime);
			}else{
			//////////////////////////////////
			sprintf(tempBuffer,"INACTIVE  %4d",ramBlowTime);
		}
		LCD_gotoXY(0,1);
		LCD_sendString(tempBuffer);
		
		break;
		
		
		case PRG_STATE_SETUP_DELAY_GATES_TIME:
		LCD_gotoXY(0,0);
		//////////////////////////////////
		LCD_sendString("MODE: GATE DELAY ");
		//////////////////////////////////
		sprintf(tempBuffer,"DELAY     %4d",delayGatesTime);
		LCD_gotoXY(0,1);
		LCD_sendString(tempBuffer);
		
		break;
		
		case PRG_STATE_SETUP_WORKING_TIME:
		LCD_gotoXY(0,0);
		//////////////////////////////////
		LCD_sendString("MODE: WORK TIME  ");
		
		//////////////////////////////////
		sprintf(tempBuffer,"TIME      %4d",workingTime);
		LCD_gotoXY(0,1);
		LCD_sendString(tempBuffer);
		
		break;
		
		case PRG_STATE_SETUP_WAITNG_TIME:
		LCD_gotoXY(0,0);
		//////////////////////////////////
		LCD_sendString("MODE: WAIT TIME  ");
		//////////////////////////////////
		sprintf(tempBuffer,"TIME      %4d",waitingTime);
		LCD_gotoXY(0,1);
		LCD_sendString(tempBuffer);
		
		break;
		
		
		
		case PRG_STATE_SETUP_AUTOMATIC_MODE:
		LCD_gotoXY(0,0);
		//////////////////////////////////
		LCD_sendString("MODE: AUTO MODE  ");
		
		if(autoCloseActive == ACTIVE){
			//////////////////////////////////
			sprintf(tempBuffer,"ACTIVE             ");
			}else{
			//////////////////////////////////
			sprintf(tempBuffer,"INACTIVE           ");
		}
		LCD_gotoXY(0,1);
		LCD_sendString(tempBuffer);
		
		break;
		
		
		case PRG_STATE_SETUP_HOLD_TIME:
		LCD_gotoXY(0,0);
		//////////////////////////////////
		LCD_sendString("MODE: HOLD MODE  ");
		//////////////////////////////////
		sprintf(tempBuffer,"HOLD TIME %4d",holdingTime);
		LCD_gotoXY(0,1);
		LCD_sendString(tempBuffer);
		
		break;
		
		case PRG_STATE_SETUP_STEP_MODE:
		LCD_gotoXY(0,0);
		//////////////////////////////////
		LCD_sendString("MODE: STEP MODE  ");
		
		
		if(stepActive == ACTIVE){
			//////////////////////////////////
			sprintf(tempBuffer,"ACTIVE             ");
			}else{
			//////////////////////////////////
			sprintf(tempBuffer,"INACTIVE           ");
		}
		LCD_gotoXY(0,1);
		LCD_sendString(tempBuffer);
		
		break;
		
		
		case PRG_STATE_SETUP_PHOTOCELL_MODE:
		LCD_gotoXY(0,0);
		//////////////////////////////////
		LCD_sendString("MODE: CELL MODE  ");
		
		if(cellActive == ACTIVE){
			//////////////////////////////////
			sprintf(tempBuffer,"ACTIVE             ");
			}else{
			//////////////////////////////////
			sprintf(tempBuffer,"INACTIVE           ");
		}
		LCD_gotoXY(0,1);
		LCD_sendString(tempBuffer);
		
		break;
		
		
		case PRG_STATE_SETUP_SAVE_PARAMETERS:
		LCD_gotoXY(0,0);
		//////////////////////////////////
		LCD_sendString("MODE: SAVING     ");
		
		LCD_gotoXY(0,1);
		LCD_sendString("                   ");
		break;
	}
	
	
}





/************************************************************************/
/* @ show lcd messages according state                                                                     */
/************************************************************************/
int showLcdMessages(uint8_t state){

	static uint8_t lastState;
	if(lastState == state) return (0);
	
	lastState = state;

	switch(state){
		case STATE_CLOSED_IDLE:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("IDLE CLOSED         ");
		
		break;
		
		case STATE_PRE_FLASHING:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("PRE FLASHING  ");
		
		break;
		
		case STATE_UNLOCK:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("UNLOCK        ");
		
		break;
		
		case STATE_OPENING_M1_RAM_PED:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1 OPEN RAM   ");
		
		break;
		
		
		case STATE_OPENING_M1_INITIAL_PED:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1 OPEN INI   ");
		
		break;
		
		case STATE_OPENING_M1_COURSE_PED:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1 OPEN COUR  ");
		
		break;
		
		
		case STATE_HOLD_OPENING_M1_PED:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1 OPEN HOLD  ");
		
		break;
		
		
		case STATE_WAITING_M1_TIME_OR_SIGNAL_PED:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1 WAITING    ");
		
		break;
		
		
		
		case STATE_CLOSING_M1_RAM_PED:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1 CLOSE RAM  ");
		
		break;
		
		
		case STATE_CLOSING_M1_INITIAL_PED:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1 CLOSE INI  ");
		
		break;
		
		
		case STATE_CLOSING_M1_COURSE_PED:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1 CLOSE COUR ");
		
		break;
		

		case STATE_HOLD_CLOSING_M1_PED:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1 CLOSE HOLD ");
		
		break;
		
		case STATE_CELL_CLOSING_M1_PED:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1 CLOSE CELL ");
		
		break;
		
		case STATE_OPENING_M1_RAM_M2_HOLD_SWING:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1 OPEN RAM M2 HOLD");
		
		break;
		
		case STATE_OPENING_M1_INITIAL_M2_HOLD_SWING:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1 OPEN INI M2 HOLD");
		
		break;
		
		case STATE_OPENING_M1_COURSE_M2_RAM_SWING:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1 OPEN COU M2 RAM ");
		
		break;
		
		
		
		case STATE_OPENING_M1_COURSE_M2_INITIAL_SWING:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1 OPEN COU M2 INI ");
		
		break;
		
		
		
		case STATE_OPENING_M1_COURSE_M2_COURSE_SWING:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1 OPEN COU M2 COU");
		
		break;
		
		
		case STATE_HOLD_OPENING_M1M2_SWING:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1M2 OPEN HOLD    ");
		
		break;
		
		case STATE_WAITING_M1M2_SIGNAL_OR_SIGNAL_SWING:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1M2 WAITING       ");
		
		break;
		
		case STATE_CLOSING_M1_HOLD_M2_RAM_SWING:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1 CLOS HOLD M2 RAM");
		
		break;
		
		case STATE_CLOSING_M1_HOLD_M2_INITIAL_SWING:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1 CLOS HOLD M2 INI");
		
		break;
		
		case STATE_CLOSING_M1_RAM_M2_COURSE_SWING:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1 CLOS RAM M2 COU ");
		break;
		
		
		case STATE_CLOSING_M1_INITIAL_M2_COURSE_SWING:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1 CLOS INI M2 COU ");
		break;
		
		
		case STATE_CLOSING_M1_COURSE_M2_COURSE_SWING:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1 CLOS COU M2 COU ");
		break;
		
		case STATE_CELL_CLOSING_M1M2_SWING:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1M2 CLOSE CELL    ");
		break;
		
		
		case STATE_HOLD_CLOSING_M1M2_SWING:
		LCD_gotoXY(0,2);
		LCD_sendString("STATUS");
		LCD_gotoXY(0,3);
		LCD_sendString("M1M2 CLOSE HOLD    ");
		break;
		
		
		
		default:
		
		break;
	}
	return (1);
}



/************************************************************************/
/* LCD STUFF                                                                     */
/************************************************************************/
void showLcdSplash(void){
	
	LCD_clr();
	_delay_ms(200);
	LCD_gotoXY(4,0);
	LCD_sendString("Gate Control");
	
	LCD_gotoXY(8,1);
	LCD_sendString("HSO");
	
	
	LCD_gotoXY(4,3);
	LCD_sendString("Version: 1.0");
	
	_delay_ms(2000); // to show the message
	
	LCD_clr();
	
}