/*
 * setpoints.h
 *
 * Created: 18.03.2016 19:52:59
 *  Author: HSO
 */ 


#ifndef SETPOINTS_H_
#define SETPOINTS_H_


/************************************************************************/
/* SETPOINTS AREA                                                                     */
/************************************************************************/
#define PREFLASHING_MAX_TIME 10
#define PREFLASHING_MIN_TIME 2
#define PREFLASHING_STEP 1


#define UNLOCK_MAX_TIME 10
#define UNLOCK_MIN_TIME 1
#define UNLOCK_STEP_TIME 1


#define WORKING_MAX_TIME 200
#define WORKING_OFFSET_TIME 5
#define WORKING_STEP_TIME 1


#define WAITING_MAX_TIME 200
#define WAITING_MIN_TIME 5
#define WAITING_STEP_TIME 1


#define HOLD_MAX_TIME 10
#define HOLD_MIN_TIME 1
#define HOLD_STEP_TIME 1


#define DELAY_GATE_MAX_TIME 10
#define DELAY_GATE_MIN_TIME 5
#define DELAY_GATE_STEP_TIME 1


#define RAM_MAX_TIME 5
#define RAM_MIN_TIME 1
#define RAM_STEP_TIME 1


extern void setTimeRamBlow(uint8_t keyUp);
extern void setTimeDelayGates(uint8_t keyUp);
extern void setTimeHold(uint8_t keyUp);
extern void setTimeWating(uint8_t keyUp);
extern void setTimeWorking(uint8_t keyUp);
extern void setTimeUnlock(uint8_t keyUp);
extern void setTimePreFlashing(uint8_t keyUp);

#endif /* SETPOINTS_H_ */