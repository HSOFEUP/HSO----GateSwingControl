/*
 * memory.c
 *
 * Created: 16.03.2016 18:34:26
 *  Author: HSO
 */ 

#include <avr/io.h>
#include <avr/eeprom.h>
#include "memory.h"

eestruct_t EEMEM eestruct_eemem;
eestruct_t eestruct_var;


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
/* load default parameters                                                                     */
/************************************************************************/
void paramLoadDefaultParameters(void){
	
	// STATES DEFAULT
	preFlashingActive = PRE_FLASH_STATE_DEFAULT;
	ramBlowActive = RAM_STATUS_DEFAULT;
	autoCloseActive = AUTO_CLOSE_STATUS_DEFAULT;
	stepActive = STEP_MODE_STATUS_DEFAULT;
	cellActive = CELL_STATUS_DEFAULT;
	
	// TIMES
	preFlashingTime = PRE_FLASH_TIME_DEFAULT;
	ramBlowTime = RAM_TIME_DEFAULT;
	unlockHoldTime = UNLOCK_TIME_DEFAULT;
	workingTime = WORKING_TIME_DEFAULT;
	waitingTime = WAITING_TIME_DEFAULT;
	holdingTime = HOLD_TIME_DEFAULT;
	delayGatesTime = DELAY_GATES_TIME_DEFAULT;
	
	powerInitial = POWER_INITIAL_DEFAULT;
	powerCourse = POWER_COURSE_DEFAULT;
}


/************************************************************************/
/* @ set initial values to eeprom  if nothin there yet                                                                   */
/************************************************************************/
void eepromSetDefaultParameters(){
	eestruct_var.initEeprom=1; // emprom init
	
	// STATUS
	eestruct_var.preFlashingActive = PRE_FLASH_STATE_DEFAULT;
	eestruct_var.ramBlowActive = RAM_STATUS_DEFAULT;
	eestruct_var.autoCloseActive = AUTO_CLOSE_STATUS_DEFAULT;
	eestruct_var.stepActive = STEP_MODE_STATUS_DEFAULT;
	eestruct_var.autoCloseActive = AUTO_CLOSE_STATUS_DEFAULT;
	
	// TIME
	eestruct_var.preFlashingTime = PRE_FLASH_TIME_DEFAULT;
	eestruct_var.unlockHoldTime = UNLOCK_TIME_DEFAULT;
	eestruct_var.ramBlowTime = RAM_TIME_DEFAULT;
	eestruct_var.workingTime = WORKING_TIME_DEFAULT;
	eestruct_var.waitingTime = WAITING_TIME_DEFAULT;
	eestruct_var.holdingTime = HOLD_TIME_DEFAULT;
	eestruct_var.delayGatesTime = DELAY_GATES_TIME_DEFAULT;
	
	eestruct_var.powerInitial = POWER_INITIAL_DEFAULT;
	eestruct_var.powerCourse = POWER_COURSE_DEFAULT;
	
	eeprom_write_block((const void*)&eestruct_var,(void*)&eestruct_eemem,sizeof(eestruct_t));
	
}


/************************************************************************/
/* @restore to EEPROM                                                                     */
/************************************************************************/
void paramLoadFromEeprom(){
	//uint8_t temp=0;
	// read from emprom
	eeprom_read_block((void*)&eestruct_var, (const void*)&eestruct_eemem,sizeof(eestruct_t));
	
	// test the fits field to check if it was written else use default and load
	if((eestruct_var.initEeprom &0xFF) ==0xFF){
		eepromSetDefaultParameters();
		paramLoadDefaultParameters();
		
	}
	else{
		// write to the global variables
		preFlashingActive = eestruct_var.preFlashingActive;
		ramBlowActive = eestruct_var.ramBlowActive;
		autoCloseActive = eestruct_var.autoCloseActive;
		stepActive = eestruct_var.stepActive;
		cellActive = eestruct_var.cellActive;
		
		// TIME
		preFlashingTime = eestruct_var.preFlashingTime;
		unlockHoldTime = eestruct_var.unlockHoldTime;
		ramBlowTime = eestruct_var.ramBlowTime;
		workingTime = eestruct_var.workingTime;
		waitingTime = eestruct_var.waitingTime;
		holdingTime = eestruct_var.holdingTime;
		delayGatesTime = eestruct_var.delayGatesTime;
		
		powerInitial = eestruct_var.powerInitial;
		powerCourse = eestruct_var.powerCourse;
		
	}
	
	
	
}

/************************************************************************/
/* @read from EEPROM                                                                     */
/************************************************************************/
void paramSavetoEeprom(){
	
	
	// save paramenetrs on the run
	eestruct_var.initEeprom=1; // emprom init
	eestruct_var.preFlashingActive = preFlashingActive;
	eestruct_var.ramBlowActive = ramBlowActive;
	eestruct_var.autoCloseActive = autoCloseActive;
	eestruct_var.stepActive = stepActive;
	eestruct_var.cellActive = cellActive;
	
	// time
	eestruct_var.preFlashingTime = preFlashingTime;
	eestruct_var.unlockHoldTime = unlockHoldTime;
	eestruct_var.ramBlowTime=ramBlowTime;
	eestruct_var.workingTime = workingTime;
	eestruct_var.waitingTime = waitingTime;
	eestruct_var.holdingTime = holdingTime;
	eestruct_var.delayGatesTime = delayGatesTime;
	
	eestruct_var.powerInitial = powerInitial;
	eestruct_var.powerCourse = powerCourse;
	
	eeprom_write_block((const void*)&eestruct_var,(void*)&eestruct_eemem,sizeof(eestruct_t));
	
	
}