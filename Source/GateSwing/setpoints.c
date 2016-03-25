/*
 * setpoints.c
 *
 * Created: 18.03.2016 19:51:48
 *  Author: HSO
 */ 


#include <inttypes.h>
#include "setpoints.h"


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
/* @ set time initial                                                                     */
/************************************************************************/
void setTimePreFlashing(uint8_t keyUp){
	if(keyUp){
		preFlashingTime +=PREFLASHING_STEP;
		if(preFlashingTime > PREFLASHING_MAX_TIME) preFlashingTime = PREFLASHING_MAX_TIME;
		}else{
		preFlashingTime -=PREFLASHING_STEP;
		if(preFlashingTime < PREFLASHING_MIN_TIME) preFlashingTime = PREFLASHING_MIN_TIME;
	}
	
}



/************************************************************************/
/* @ unlock time                                                                     */
/************************************************************************/
void setTimeUnlock(uint8_t keyUp){
	if(keyUp){
		unlockHoldTime +=UNLOCK_STEP_TIME;
		if(unlockHoldTime > UNLOCK_MAX_TIME) unlockHoldTime = UNLOCK_MAX_TIME;
		}else{
		unlockHoldTime -=UNLOCK_STEP_TIME;
		if(unlockHoldTime < UNLOCK_MIN_TIME) unlockHoldTime = UNLOCK_MIN_TIME;
	}
	
}


/************************************************************************/
/* @ set working time Initial + time                                                                     */
/************************************************************************/
void setTimeWorking(uint8_t keyUp){
	if(keyUp){
		workingTime +=WORKING_STEP_TIME;
		if(workingTime > WORKING_MAX_TIME) workingTime = WORKING_MAX_TIME;
		}else{
		workingTime -=UNLOCK_STEP_TIME;
		if(workingTime < delayGatesTime+ WORKING_OFFSET_TIME) workingTime = delayGatesTime + WORKING_OFFSET_TIME;
	}
	
	
}



/************************************************************************/
/* @ set wating time                                                                     */
/************************************************************************/
void setTimeWating(uint8_t keyUp){
	if(keyUp){
		waitingTime +=WAITING_STEP_TIME;
		if(waitingTime > WAITING_MAX_TIME) waitingTime = WAITING_MAX_TIME;
		}else{
		waitingTime -=UNLOCK_STEP_TIME;
		if(waitingTime < WAITING_MIN_TIME) waitingTime = WAITING_MIN_TIME;
	}
}




/************************************************************************/
/* @ set hold time                                                                     */
/************************************************************************/
void setTimeHold(uint8_t keyUp){
	if(keyUp){
		holdingTime +=HOLD_STEP_TIME;
		if(holdingTime > HOLD_MAX_TIME) holdingTime = HOLD_MAX_TIME;
		}else{
		holdingTime -=HOLD_STEP_TIME;
		if(holdingTime < HOLD_MIN_TIME) holdingTime = HOLD_MIN_TIME;
	}
}


/************************************************************************/
/* @ set delay gates                                                                     */
/************************************************************************/
void setTimeDelayGates(uint8_t keyUp){
	if(keyUp){
		delayGatesTime +=DELAY_GATE_STEP_TIME;
		if(delayGatesTime > DELAY_GATE_MAX_TIME) delayGatesTime = DELAY_GATE_MAX_TIME;
		}else{
		delayGatesTime -=DELAY_GATE_STEP_TIME;
		if(delayGatesTime < DELAY_GATE_MIN_TIME) delayGatesTime = DELAY_GATE_MIN_TIME;
	}
}



/************************************************************************/
/* set time RAM                                                                     */
/************************************************************************/
void setTimeRamBlow(uint8_t keyUp){
	if(keyUp){
		ramBlowTime +=RAM_STEP_TIME;
		if(ramBlowTime > RAM_MAX_TIME) ramBlowTime = RAM_MAX_TIME;
		}else{
		ramBlowTime -=RAM_STEP_TIME;
		if(ramBlowTime < RAM_MIN_TIME) ramBlowTime = RAM_MIN_TIME;
	}
}
