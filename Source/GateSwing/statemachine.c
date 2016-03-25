/*
 * statemachine.c
 *
 * Created: 16.03.2016 18:35:50
 *  Author: HSO
 */ 


#include <inttypes.h>
#include "iodefines.h"
#include "statemachine.h"
#include "power.h"
#include "setpoints.h"

#include "libs/usart/usart.h"

/************************************************************************/
/* DEBUG                                                                     */
/************************************************************************/
//#define DEBUG_STATE
char debugBuffer[10];


// VARS
extern uint8_t stateSwing;
extern uint8_t stateProgram;

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

extern volatile uint8_t photoCellsState;
extern volatile uint8_t fca1State;
extern volatile uint8_t fcc1State;
extern volatile uint8_t fca2State;
extern volatile uint8_t fcc2State;
extern volatile uint8_t stopState;
extern volatile uint8_t aux1State;
extern volatile uint8_t aux2State;



extern volatile uint8_t flashingActive;


extern volatile uint8_t flagSaveMemory;


/************************************************************************/
/* STATE MACHINE SWING        Called 1 second period                                                             */
/************************************************************************/
int stateMachineSwing(uint8_t sig){
	
	static uint16_t runningTime;
	static uint16_t savedRunningTime;
	static uint16_t timming;
	static uint8_t swing;
	runningTime++;
	
	switch(stateSwing){
		
		case STATE_CLOSED_IDLE:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_CLOSED_IDLE\n\r");
		#endif
		
		runningTime=0;
		swing =INACTIVE;
		
		if(sig == SIGNAL_REMOTE_PED){ // if pedonal signal
			
			swing =INACTIVE;
			if(preFlashingActive==ACTIVE)	stateSwing = STATE_PRE_FLASHING;
			else stateSwing = STATE_UNLOCK;
			return(1);
			
		}
		if(sig == SIGNAL_REMOTE_SWING){ // if both dorrs
			swing=ACTIVE;
			if(preFlashingActive==ACTIVE)	stateSwing = STATE_PRE_FLASHING;
			else stateSwing = STATE_UNLOCK;
			return(1);
		}
		
		return(0);
		break;
		
		case STATE_PRE_FLASHING:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_PRE_FLASHING\n\r");
		#endif
		
		
		timming++;
		if(timming >preFlashingTime ){
			timming=0; // reset timer
			runningTime=0;
			stateSwing = STATE_UNLOCK;
			return(1);
			
		}
		return(0);
		break;
		
		
		case STATE_UNLOCK:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_UNLOCK\n\r");
		#endif
		
		timming++;
		if(timming >unlockHoldTime){
			timming=0; // reset timer
			runningTime=0;
			if(swing == INACTIVE){ // PED
				if(ramBlowActive==ACTIVE) stateSwing = STATE_OPENING_M1_RAM_PED;
				else stateSwing = STATE_OPENING_M1_INITIAL_PED;
				
				return(1);
			}
			// swing MODE
			if(swing == ACTIVE){
				if(ramBlowActive == ACTIVE) stateSwing = STATE_OPENING_M1_RAM_M2_HOLD_SWING;
				else stateSwing = STATE_OPENING_M1_INITIAL_M2_HOLD_SWING;
				
				return (1);
			}
		}
		return(0);
		break;
		
		case STATE_OPENING_M1_RAM_PED:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_OPENING_M1_RAM_PED\n\r");
		#endif
		
		timming++;
		if(timming > ramBlowTime){
			timming=0; // reset timer
			runningTime=0;
			stateSwing = STATE_OPENING_M1_INITIAL_PED;
			return(1);
		}
		return (0);
		break;
		
		
		case STATE_OPENING_M1_INITIAL_PED:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_OPENING_M1_INITIAL_PED\n\r");
		#endif
		
		// RADIO SIGNAL RECEIVED
		if(sig == SIGNAL_REMOTE_PED || sig == SIGNAL_REMOTE_SWING){
			savedRunningTime = runningTime; // SAVE Running time
			timming=0;
			runningTime=0;
			stateSwing = STATE_HOLD_OPENING_M1_PED;
			return (1);
		}
		
		
		// DELAY MODE INITIAL OPENING
		timming++;
		if(timming > delayGatesTime){
			
			timming=0; // reset timming
			stateSwing = STATE_OPENING_M1_COURSE_PED;
			return(1);
		}
		
		
		
		return(0);
		break;
		
		case STATE_OPENING_M1_COURSE_PED:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_OPENING_M1_COURSE_PED\n\r");
		#endif
		
		// RECEIVED RADIO SIGNAL
		if(sig == SIGNAL_REMOTE_PED || sig == SIGNAL_REMOTE_SWING){
			savedRunningTime = runningTime; // SAVE Running time
			timming=0;
			stateSwing=STATE_HOLD_OPENING_M1_PED;
			return(1);
		}
		
		
		
		// REACH WORKING TIME OPENING
		if(runningTime > workingTime){
			runningTime=0; // reset time
			timming=0;
			stateSwing = STATE_WAITING_M1_TIME_OR_SIGNAL_PED;
			return(1);
		}
		
		
		
		return(0);
		break;
		
		
		
		case STATE_HOLD_OPENING_M1_PED:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_HOLD_OPENING_M1_PED\n\r");
		#endif
		
		// ONE SIGNAL REVERSE
		if(stepActive==INACTIVE){
			if(cellActive == ACTIVE){
				if(photoCellsState == CELL_FREE){// CELL ACTIVE AND BLOCKED REMAIN HERE
					timming++;
					if(timming > holdingTime){
						timming=0;
						runningTime= workingTime - savedRunningTime;
						stateSwing = STATE_CLOSING_M1_COURSE_PED;
					}
				}
				}else{
				timming++;
				if(timming > holdingTime){
					timming=0;
					runningTime=workingTime - savedRunningTime;;
					stateSwing = STATE_CLOSING_M1_COURSE_PED;
					return (1);
				}
				
			}
			}else{ // STEP BY STEP JUST HOLD TO SEND TO WAITING
			timming++;
			if(timming > holdingTime){
				stateSwing = STATE_WAITING_M1_TIME_OR_SIGNAL_PED;
				timming=0;
				runningTime=0;
			}
			return (1);
		}
		return (0);
		break;
		
		case STATE_WAITING_M1_TIME_OR_SIGNAL_PED:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_WAITING_M1_TIME_OR_SIGNAL_PED\n\r");
		#endif
		
		//STOP THE DOOR CELL BLOCKED
		if(cellActive == ACTIVE && !photoCellsState == CELL_FREE){
			timming =0;
			runningTime=0;
			#ifdef DEBUG_STATE
			USART1_sendStr("BLOCK");
			#endif
			stateSwing = STATE_WAITING_M1_TIME_OR_SIGNAL_PED;
			return(1);
		}
		
		
		// IF ALL CLEAR PROCEED
		if(autoCloseActive == ACTIVE){
			runningTime++;
			if(runningTime > waitingTime || sig == SIGNAL_REMOTE_PED || sig == SIGNAL_REMOTE_SWING){
				runningTime=0;
				timming=0;
				if(ramBlowActive == ACTIVE) stateSwing = STATE_CLOSING_M1_RAM_PED;
				else stateSwing = STATE_CLOSING_M1_INITIAL_PED;
			}
			return (1);
		}
		if(autoCloseActive == INACTIVE){
			if(sig == SIGNAL_REMOTE_PED || sig == SIGNAL_REMOTE_SWING){
				runningTime=0;
				timming=0;
				if(ramBlowActive == ACTIVE) stateSwing = STATE_CLOSING_M1_RAM_PED;
				else stateSwing = STATE_CLOSING_M1_INITIAL_PED;
			}
			return (1);
		}
		
		return(0);
		break;
		
		case STATE_CLOSING_M1_RAM_PED:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_CLOSING_M1_RAM_PED\n\r");
		#endif
		
		timming++;
		if(timming > ramBlowTime){
			timming=0;
			runningTime=0;
			stateSwing = STATE_CLOSING_M1_INITIAL_PED;
			return(1);
		}
		return(0);
		break;
		
		case STATE_CLOSING_M1_INITIAL_PED:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_CLOSING_M1_INITIAL_PED\n\r");
		#endif
		
		//STOP THE DOOR CELL BLOCKED
		if(cellActive == ACTIVE && !photoCellsState == CELL_FREE){
			savedRunningTime = runningTime;
			timming =0;
			runningTime=0;
			#ifdef DEBUG_STATE
			USART1_sendStr("BLOCK");
			#endif
			stateSwing = STATE_CELL_CLOSING_M1_PED;
			return(1);
		}
		
		// REMOTE SIGNAL RECEIVED
		if(sig == SIGNAL_REMOTE_PED || sig == SIGNAL_REMOTE_SWING){
			savedRunningTime = runningTime;
			timming=0;
			runningTime=0;
			stateSwing=STATE_HOLD_CLOSING_M1_PED;
			return(1);
		}
		
		
		timming++;
		if(timming>delayGatesTime){
			timming=0;
			stateSwing=STATE_CLOSING_M1_COURSE_PED;
			return(0);
		}
		break;
		
		case STATE_CLOSING_M1_COURSE_PED:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_CLOSING_M1_COURSE_PED\n\r");
		#endif
		
		
		//STOP THE DOOR CELL BLOCKED
		if(cellActive == ACTIVE && !photoCellsState == CELL_FREE){
			savedRunningTime = runningTime;
			timming =0;
			runningTime=0;
			#ifdef DEBUG_STATE
			USART1_sendStr("BLOCK");
			#endif
			stateSwing = STATE_CELL_CLOSING_M1_PED;
			return(1);
		}
		
		
		// STOP THE DOOR SIGNAL HOLD
		if(sig == SIGNAL_REMOTE_PED || sig == SIGNAL_REMOTE_SWING){
			savedRunningTime = runningTime;
			timming=0;
			stateSwing=STATE_HOLD_CLOSING_M1_PED;
			return(1);
		}
		
		
		// RUNNING TIME REACHED CLOSING
		if(runningTime > workingTime){
			runningTime=0;
			timming=0;
			stateSwing = STATE_CLOSED_IDLE;
			return(1);
		}
		
		
		return(0);
		break;
		
		case STATE_CELL_CLOSING_M1_PED:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_CELL_CLOSING_M1_PED\n\r");
		#endif
		timming++;
		if(timming>holdingTime){
			runningTime = workingTime - savedRunningTime;
			stateSwing = STATE_OPENING_M1_COURSE_PED;
			return(1);
		}
		return(0);
		
		break;
		
		case STATE_HOLD_CLOSING_M1_PED: // photocell activated
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_HOLD_CLOSING_M1_PED\n\r");
		#endif
		
		timming++;
		if(stepActive == INACTIVE && timming >holdingTime){
			runningTime = workingTime - savedRunningTime;
			timming=0;
			stateSwing = STATE_OPENING_M1_COURSE_PED;
			return(1);
		}
		if(stepActive == ACTIVE){
			timming = 0;
			if(sig == SIGNAL_REMOTE_PED){
				runningTime = workingTime - savedRunningTime;
				stateSwing = STATE_OPENING_M1_COURSE_PED;
				return(1);
			}
			if(sig == SIGNAL_REMOTE_SWING){
				if(ramBlowActive == ACTIVE) stateSwing = STATE_OPENING_M1_COURSE_M2_RAM_SWING;
				else stateSwing = STATE_OPENING_M1_COURSE_M2_INITIAL_SWING;
			}
			
		}
		return(0);
		break;
		
		/************************************************************************/
		/* SWING MODES                                                                     */
		/************************************************************************/
		case STATE_OPENING_M1_RAM_M2_HOLD_SWING:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE_OPENING_M1_RAM_M2_HOLD_SWING\n\r");
		#endif
		
		timming++;
		if(timming > ramBlowTime){
			timming=0; // reset timer
			runningTime=0;
			stateSwing = STATE_OPENING_M1_INITIAL_M2_HOLD_SWING;
			return(1);
		}
		return (0);
		break;
		
		
		
		case STATE_OPENING_M1_INITIAL_M2_HOLD_SWING:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE_OPENING_M1_INITIAL_M2_HOLD_SWING\n\r");
		#endif
		
		
		
		// RADIO SIGNAL RECEIVED
		if(sig == SIGNAL_REMOTE_PED || sig == SIGNAL_REMOTE_SWING){
			savedRunningTime = runningTime; // SAVE Running time
			timming=0;
			runningTime=0;
			stateSwing = STATE_HOLD_OPENING_M1M2_SWING;
			return (1);
		}
		
		
		// DELAY MODE INITIAL OPENING
		timming++;
		if(timming > delayGatesTime){
			timming=0; // reset timming
			if(ramBlowActive == ACTIVE) stateSwing = STATE_OPENING_M1_COURSE_M2_RAM_SWING;
			else stateSwing = STATE_OPENING_M1_COURSE_M2_INITIAL_SWING;
			return(1);
		}
		
		
		return(0);
		break;
		
		
		case STATE_OPENING_M1_COURSE_M2_RAM_SWING:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_OPENING_M1_COURSE_M2_RAM_SWING\n\r");
		#endif
		
		timming++;
		if(timming > ramBlowTime){
			timming=0; // reset timer
			runningTime=0;
			stateSwing = STATE_OPENING_M1_COURSE_M2_INITIAL_SWING;
			return(1);
		}
		
		// RECEIVED RADIO SIGNAL
		if(sig == SIGNAL_REMOTE_PED || sig == SIGNAL_REMOTE_SWING){
			savedRunningTime = runningTime; // SAVE Running time
			timming=0;
			stateSwing=STATE_HOLD_OPENING_M1M2_SWING;
			return(1);
		}
		
		return(0);
		break;
		
		
		case STATE_OPENING_M1_COURSE_M2_INITIAL_SWING:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_OPENING_M1_COURSE_M2_INITIAL_SWING\n\r");
		#endif
		
		// RECEIVED RADIO SIGNAL
		if(sig == SIGNAL_REMOTE_PED || sig == SIGNAL_REMOTE_SWING){
			savedRunningTime = runningTime; // SAVE Running time
			timming=0;
			stateSwing=STATE_HOLD_OPENING_M1M2_SWING;
			return(1);
		}
		
		// DELAY MODE INITIAL OPENING
		timming++;
		if(timming > delayGatesTime){
			timming=0; // reset timming
			stateSwing = STATE_OPENING_M1_COURSE_M2_COURSE_SWING;
			return(1);
		}
		
		
		return(0);
		break;
		
		case STATE_OPENING_M1_COURSE_M2_COURSE_SWING:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_OPENING_M1_COURSE_M2_COURSE_SWING\n\r");
		#endif
		
		// RECEIVED RADIO SIGNAL
		if(sig == SIGNAL_REMOTE_PED || sig == SIGNAL_REMOTE_SWING){
			savedRunningTime = runningTime; // SAVE Running time
			timming=0;
			stateSwing=STATE_HOLD_OPENING_M1M2_SWING;
			return(1);
		}
		
		// REACH WORKING TIME OPENING
		if(runningTime > workingTime){
			runningTime=0; // reset time
			timming=0;
			stateSwing = STATE_WAITING_M1M2_SIGNAL_OR_SIGNAL_SWING;
			return(1);
		}
		return(0);
		
		break;
		
		
		case STATE_HOLD_OPENING_M1M2_SWING:
		
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_HOLD_OPENING_M1M2_SWING\n\r");
		#endif
		
		// ONE SIGNAL REVERSE
		if(stepActive==INACTIVE){
			if(cellActive == ACTIVE){
				if(photoCellsState == CELL_FREE){// CELL ACTIVE AND BLOCKED REMAIN HERE
					timming++;
					if(timming > holdingTime){
						timming=0;
						runningTime= workingTime - savedRunningTime;
						stateSwing = STATE_CLOSING_M1_COURSE_M2_COURSE_SWING;
					}
				}
				}else{
				timming++;
				if(timming > holdingTime){
					timming=0;
					runningTime=workingTime - savedRunningTime;;
					stateSwing = STATE_CLOSING_M1_COURSE_M2_COURSE_SWING;
					return (1);
				}
				
			}
			}else{ // STEP BY STEP JUST HOLD TO SEND TO WAITING
			timming++;
			if(timming > holdingTime){
				stateSwing = STATE_WAITING_M1M2_SIGNAL_OR_SIGNAL_SWING;
				timming=0;
				runningTime=0;
			}
			return (1);
		}
		return (0);
		
		break;
		
		case STATE_WAITING_M1M2_SIGNAL_OR_SIGNAL_SWING:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_WAITING_M1M2_SIGNAL_OR_SIGNAL_SWING\n\r");
		#endif
		
		//STOP THE DOOR CELL BLOCKED
		if(cellActive == ACTIVE && !photoCellsState == CELL_FREE){
			timming =0;
			runningTime=0;
			#ifdef DEBUG_STATE
			USART1_sendStr("BLOCK");
			#endif
			stateSwing = STATE_WAITING_M1M2_SIGNAL_OR_SIGNAL_SWING;
			return(1);
		}
		
		
		// IF ALL CLEAR PROCEED
		if(autoCloseActive == ACTIVE){
			runningTime++;
			if(runningTime > waitingTime || sig == SIGNAL_REMOTE_PED || sig == SIGNAL_REMOTE_SWING){
				runningTime=0;
				timming=0;
				if(ramBlowActive == ACTIVE) stateSwing = STATE_CLOSING_M1_HOLD_M2_RAM_SWING;
				else stateSwing = STATE_CLOSING_M1_HOLD_M2_INITIAL_SWING;
			}
			return (1);
		}
		if(autoCloseActive == INACTIVE){
			if(sig == SIGNAL_REMOTE_PED || sig == SIGNAL_REMOTE_SWING){
				runningTime=0;
				timming=0;
				if(ramBlowActive == ACTIVE) stateSwing = STATE_CLOSING_M1_HOLD_M2_RAM_SWING;
				else stateSwing = STATE_CLOSING_M1_HOLD_M2_INITIAL_SWING;
			}
			return (1);
		}
		
		return(0);
		break;
		
		
		case STATE_CLOSING_M1_HOLD_M2_RAM_SWING:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_CLOSING_M1_HOLD_M2_RAM_SWING\n\r");
		#endif
		
		timming++;
		if(timming > ramBlowTime){
			timming=0;
			runningTime=0;
			stateSwing = STATE_CLOSING_M1_HOLD_M2_INITIAL_SWING;
			return(1);
		}
		return(0);
		break;
		
		case STATE_CLOSING_M1_HOLD_M2_INITIAL_SWING:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_CLOSING_M1_HOLD_M2_INITIAL_SWING\n\r");
		#endif
		
		//STOP THE DOOR CELL BLOCKED
		if(cellActive == ACTIVE && !photoCellsState == CELL_FREE){
			savedRunningTime = runningTime;
			timming =0;
			runningTime=0;
			#ifdef DEBUG_STATE
			USART1_sendStr("BLOCK");
			#endif
			stateSwing = STATE_CELL_CLOSING_M1M2_SWING;
			return(1);
		}
		
		// REMOTE SIGNAL RECEIVED
		if(sig == SIGNAL_REMOTE_PED || sig == SIGNAL_REMOTE_SWING){
			savedRunningTime = runningTime;
			timming=0;
			runningTime=0;
			stateSwing=STATE_HOLD_CLOSING_M1M2_SWING;
			return(1);
		}
		
		
		timming++;
		if(timming>delayGatesTime){
			timming=0;
			if(ramBlowActive == ACTIVE)stateSwing = STATE_CLOSING_M1_RAM_M2_COURSE_SWING;
			else stateSwing=STATE_CLOSING_M1_INITIAL_M2_COURSE_SWING;
			return(1);
		}
		return (0);
		break;
		
		
		case STATE_CLOSING_M1_RAM_M2_COURSE_SWING:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_CLOSING_M1_RAM_M2_COURSE_SWING\n\r");
		#endif
		
		//STOP THE DOOR CELL BLOCKED
		if(cellActive == ACTIVE && !photoCellsState == CELL_FREE){
			savedRunningTime = runningTime;
			timming =0;
			runningTime=0;
			#ifdef DEBUG_STATE
			USART1_sendStr("BLOCK");
			#endif
			stateSwing = STATE_CELL_CLOSING_M1M2_SWING;
			return(1);
		}
		
		
		timming++;
		if(timming > ramBlowTime){
			timming=0;
			runningTime=0;
			stateSwing = STATE_CLOSING_M1_INITIAL_M2_COURSE_SWING;
			return(1);
		}
		return(0);
		
		break;
		
		
		case STATE_CLOSING_M1_INITIAL_M2_COURSE_SWING:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_CLOSING_M1_INITIAL_M2_COURSE_SWING\n\r");
		#endif
		
		//STOP THE DOOR CELL BLOCKED
		if(cellActive == ACTIVE && !photoCellsState == CELL_FREE){
			savedRunningTime = runningTime;
			timming =0;
			runningTime=0;
			#ifdef DEBUG_STATE
			USART1_sendStr("BLOCK");
			#endif
			stateSwing = STATE_CELL_CLOSING_M1M2_SWING;
			return(1);
		}
		
		// REMOTE SIGNAL RECEIVED
		if(sig == SIGNAL_REMOTE_PED || sig == SIGNAL_REMOTE_SWING){
			savedRunningTime = runningTime;
			timming=0;
			runningTime=0;
			stateSwing=STATE_HOLD_CLOSING_M1M2_SWING;
			return(1);
		}
		
		
		timming++;
		if(timming>delayGatesTime){
			timming=0;
			stateSwing = STATE_CLOSING_M1_COURSE_M2_COURSE_SWING;
			return(1);
		}
		return (0);
		
		break;
		
		
		case STATE_CLOSING_M1_COURSE_M2_COURSE_SWING:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_CLOSING_M1_COURSE_M2_COURSE_SWING\n\r");
		#endif
		
		//STOP THE DOOR CELL BLOCKED
		if(cellActive == ACTIVE && !photoCellsState == CELL_FREE){
			savedRunningTime = runningTime;
			timming =0;
			runningTime=0;
			#ifdef DEBUG_STATE
			USART1_sendStr("BLOCK");
			#endif
			stateSwing = STATE_CELL_CLOSING_M1M2_SWING;
			return(1);
		}
		
		// REMOTE SIGNAL RECEIVED
		if(sig == SIGNAL_REMOTE_PED || sig == SIGNAL_REMOTE_SWING){
			savedRunningTime = runningTime;
			timming=0;
			runningTime=0;
			stateSwing=STATE_HOLD_CLOSING_M1M2_SWING;
			return(1);
		}
		
		if(runningTime > workingTime){
			timming=0;
			runningTime=0;
			stateSwing = STATE_CLOSED_IDLE;
		}
		
		break;
		
		case STATE_CELL_CLOSING_M1M2_SWING:
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_CELL_CLOSING_M1M2_SWING\n\r");
		#endif
		timming++;
		if(timming>holdingTime){
			runningTime = workingTime - savedRunningTime;
			stateSwing = STATE_OPENING_M1_COURSE_M2_COURSE_SWING;
			return(1);
		}
		return(0);
		
		break;
		
		case STATE_HOLD_CLOSING_M1M2_SWING: // photocell activated
		#ifdef DEBUG_STATE
		USART1_sendStr("STATE: STATE_HOLD_CLOSING_M1M2_SWING\n\r");
		#endif
		
		timming++;
		if(stepActive == INACTIVE && timming >holdingTime){
			runningTime = workingTime - savedRunningTime;
			timming=0;
			stateSwing = STATE_OPENING_M1_COURSE_M2_COURSE_SWING;
			return(1);
		}
		if(stepActive == ACTIVE){
			timming = 0;
			if(sig == SIGNAL_REMOTE_PED || sig == SIGNAL_REMOTE_SWING){
				runningTime = workingTime - savedRunningTime;
				stateSwing = STATE_OPENING_M1_COURSE_M2_COURSE_SWING;
				return(1);
			}
			
		}
		return(0);
		break;
		
		
		
		default:
		stateSwing=STATE_CLOSED_IDLE;
		return(1);
		break;
		
	}
	
	return(0);
}




/************************************************************************/
/* set outputs according machine state                                                                     */
/************************************************************************/
int decodeStateMachine(uint8_t state){
	
	static uint8_t lastState;
	if(lastState == state) return (0);
	
	lastState = state;
	switch(state){
		case STATE_CLOSED_IDLE:
		
		IDLE_M1;
		IDLE_M2;
		IDLE_UNLOCK;
		flashingActive=INACTIVE;
		setMotorPower(0,0);
		IDLE_POWER_CELLS; // swicth down cells
		
		break;
		
		case STATE_PRE_FLASHING:
		IDLE_M1;
		IDLE_M2;
		IDLE_UNLOCK;
		flashingActive=ACTIVE;
		break;
		
		case STATE_UNLOCK:
		SET_UNLOCK;
		IDLE_M1;
		IDLE_M2;
		flashingActive=ACTIVE;
		break;
		
		case STATE_OPENING_M1_RAM_PED:
		setMotorPower(powerInitial,0);
		if(!fca1State == REED_ACTIVE)IDLE_M1;
		else CLOSE_M1;
		
		IDLE_M2;
		SET_UNLOCK;
		flashingActive=ACTIVE;
		
		break;
		
		case STATE_OPENING_M1_INITIAL_PED:
		setMotorPower(powerInitial,0);
		
		IDLE_M2;
		if(!fca1State == REED_ACTIVE)IDLE_M1;
		else OPEN_M1;
		SET_UNLOCK;
		flashingActive=ACTIVE;
		break;
		
		
		case STATE_OPENING_M1_COURSE_PED:
		setMotorPower(powerCourse,0);
		if(!fca1State == REED_ACTIVE)IDLE_M1;
		else OPEN_M1;
		IDLE_UNLOCK;
		flashingActive=ACTIVE;
		break;
		
		case STATE_WAITING_M1_TIME_OR_SIGNAL_PED:
		setMotorPower(0,0);
		IDLE_M1;
		IDLE_M2;
		flashingActive=INACTIVE;
		if(cellActive==ACTIVE){
			SET_POWER_CELLS;
		}else IDLE_POWER_CELLS;
		
		break;
		
		case STATE_CLOSING_M1_RAM_PED:
		setMotorPower(powerInitial,0);
		if(!fcc1State == REED_ACTIVE)IDLE_M1;
		else OPEN_M1;
		IDLE_M2;
		flashingActive=ACTIVE;
		break;
		
		case STATE_CLOSING_M1_INITIAL_PED:
		setMotorPower(powerInitial,0);
		if(!fcc1State == REED_ACTIVE)IDLE_M1;
		else CLOSE_M1;
		
		IDLE_M2;
		flashingActive=ACTIVE;
		break;
		
		case STATE_CLOSING_M1_COURSE_PED:
		setMotorPower(powerCourse,0);
		if(!fcc1State == REED_ACTIVE)IDLE_M1;
		else CLOSE_M1;
		IDLE_M2;
		flashingActive=ACTIVE;
		break;
		
		
		case STATE_HOLD_CLOSING_M1_PED:
		setMotorPower(0,0);
		IDLE_M1;
		IDLE_M2;
		flashingActive = INACTIVE;
		
		break;
		
		
		case STATE_CELL_CLOSING_M1_PED:
		setMotorPower(0,0);
		IDLE_M2;
		IDLE_M1;
		flashingActive=ACTIVE;
		break;
		
		
		case STATE_HOLD_OPENING_M1_PED:
		
		// ENABLE CELL IF STEP and CELL
		if(cellActive == ACTIVE){
			SET_POWER_CELLS;
		}else IDLE_POWER_CELLS;
		
		
		if(stepActive == ACTIVE){
			flashingActive =INACTIVE;
		}else flashingActive = ACTIVE;
		
		IDLE_M1;
		IDLE_M2;
		
		break;
		
		
		default:
		break;
		
		case STATE_OPENING_M1_RAM_M2_HOLD_SWING:
		if(!fca1State == REED_ACTIVE)IDLE_M1;
		else CLOSE_M1;
		IDLE_M2;
		flashingActive=ACTIVE;
		
		break;
		
		case STATE_OPENING_M1_INITIAL_M2_HOLD_SWING:
		if(!fca1State == REED_ACTIVE)IDLE_M1;
		else OPEN_M1;
		IDLE_M2;
		flashingActive=ACTIVE;
		
		break;
		
		case STATE_OPENING_M1_COURSE_M2_RAM_SWING:
		if(!fca1State == REED_ACTIVE)IDLE_M1;
		else OPEN_M1;
		
		if(!fca2State == REED_ACTIVE)IDLE_M2;
		else CLOSE_M2;
		flashingActive=ACTIVE;
		IDLE_UNLOCK;
		
		break;
		
		
		case STATE_OPENING_M1_COURSE_M2_INITIAL_SWING:
		if(!fca1State == REED_ACTIVE)IDLE_M1;
		else OPEN_M1;
		
		if(!fca2State == REED_ACTIVE)IDLE_M2;
		else OPEN_M2;
		flashingActive=ACTIVE;
		IDLE_UNLOCK;
		break;
		
		case STATE_OPENING_M1_COURSE_M2_COURSE_SWING:
		if(!fca1State == REED_ACTIVE)IDLE_M1;
		else OPEN_M1;
		
		if(!fca2State == REED_ACTIVE)IDLE_M2;
		else OPEN_M2;
		flashingActive=ACTIVE;
		break;
		
		case STATE_WAITING_M1M2_SIGNAL_OR_SIGNAL_SWING:
		if(cellActive == ACTIVE)SET_POWER_CELLS;
		else IDLE_POWER_CELLS;
		IDLE_M1;
		IDLE_M2;
		flashingActive=INACTIVE;
		
		break;
		
		case STATE_CLOSING_M1_HOLD_M2_RAM_SWING:
		if(!fcc2State == REED_ACTIVE)IDLE_M2;
		else OPEN_M2;
		
		flashingActive=ACTIVE;
		break;
		
		case STATE_CLOSING_M1_HOLD_M2_INITIAL_SWING:
		if(!fcc2State == REED_ACTIVE)IDLE_M2;
		else CLOSE_M2;
		
		flashingActive=ACTIVE;
		break;
		
		
		case STATE_CLOSING_M1_RAM_M2_COURSE_SWING:
		if(!fcc2State == REED_ACTIVE)IDLE_M2;
		else CLOSE_M2;
		
		if(!fcc1State == REED_ACTIVE)IDLE_M1;
		else OPEN_M1;
		
		flashingActive=ACTIVE;
		
		break;
		
		case STATE_CLOSING_M1_INITIAL_M2_COURSE_SWING:
		if(!fcc2State == REED_ACTIVE)IDLE_M2;
		else CLOSE_M2;
		
		if(!fcc1State == REED_ACTIVE)IDLE_M1;
		else CLOSE_M1;
		
		flashingActive=ACTIVE;
		
		break;
		
		case STATE_CLOSING_M1_COURSE_M2_COURSE_SWING:
		if(!fcc2State == REED_ACTIVE)IDLE_M2;
		else CLOSE_M2;
		
		if(!fcc1State == REED_ACTIVE)IDLE_M1;
		else CLOSE_M1;
		
		flashingActive=ACTIVE;
		
		break;
		
		case STATE_HOLD_CLOSING_M1M2_SWING:
		if(cellActive == ACTIVE)SET_POWER_CELLS;
		else IDLE_POWER_CELLS;
		IDLE_M1;
		IDLE_M2;
		
		flashingActive = INACTIVE;
		break;
		
		case STATE_CELL_CLOSING_M1M2_SWING:
		if(cellActive == ACTIVE)SET_POWER_CELLS;
		else IDLE_POWER_CELLS;
		IDLE_M1;
		IDLE_M2;
		
		flashingActive = ACTIVE;
		break;
		
		
	}
	return(1);
}


/************************************************************************/
/* @ State Machine Program                                                                     */
/************************************************************************/
void stateMachineProgram (uint8_t sig){
	
	
	switch(stateProgram){
		
		case PRG_STATE_RUN:
		if(sig == BUTTON_PRG) stateProgram = PRG_STATE_SETUP_POWER_INI;
		break;
		
		
		case PRG_STATE_SETUP_POWER_INI:
		#ifdef DEBUG_STATE
		USART1_sendStr("PRG : POWER_INI\n\r");
		#endif
		
		if(sig == BUTTON_UP) setPWMInitial(1);
		if(sig == BUTTON_DOWN) setPWMInitial(0);
		if(sig == BUTTON_ENTER) stateProgram = PRG_STATE_SETUP_POWER_COU;
		if(sig == BUTTON_ESC) stateProgram = PRG_STATE_SETUP_SAVE_PARAMETERS;
		break;
		
		case PRG_STATE_SETUP_POWER_COU:
		#ifdef DEBUG_STATE
		USART1_sendStr("PRG : POWER_COU\n\r");
		#endif
		
		if(sig == BUTTON_UP) setPWMCourse(1);
		if(sig == BUTTON_DOWN) setPWMCourse(0);
		if(sig == BUTTON_ENTER) stateProgram = PRG_STATE_SETUP_PREFLASHING_MODE;
		if(sig == BUTTON_ESC) stateProgram = PRG_STATE_SETUP_SAVE_PARAMETERS;
		break;
		
		case PRG_STATE_SETUP_PREFLASHING_MODE:
		#ifdef DEBUG_STATE
		USART1_sendStr("PRG : PRE_FLASH_MODE\n\r");
		#endif
		
		if(sig == BUTTON_UP) preFlashingActive=1;
		if(sig == BUTTON_DOWN) preFlashingActive=0;
		if(preFlashingActive == ACTIVE && sig == BUTTON_ENTER) stateProgram = PRG_STATE_SETUP_PREFLASHING_TIME;
		if(preFlashingActive == INACTIVE && sig == BUTTON_ENTER) stateProgram = PRG_STATE_SETUP_UNLOCK_TIME;
		if(sig == BUTTON_ESC) stateProgram = PRG_STATE_SETUP_SAVE_PARAMETERS;
		
		break;
		
		
		case PRG_STATE_SETUP_PREFLASHING_TIME:
		if(sig == BUTTON_UP) setTimePreFlashing(1);
		if(sig == BUTTON_DOWN) setTimePreFlashing(0);
		if(sig == BUTTON_ENTER) stateProgram = PRG_STATE_SETUP_UNLOCK_TIME;
		if(sig == BUTTON_ESC) stateProgram = PRG_STATE_SETUP_SAVE_PARAMETERS;
		
		break;
		
		case PRG_STATE_SETUP_UNLOCK_TIME:
		#ifdef DEBUG_STATE
		USART1_sendStr("PRG : UNLOCK_TIME\n\r");
		#endif
		
		if(sig == BUTTON_UP) setTimeUnlock(1);
		if(sig == BUTTON_DOWN) setTimeUnlock(0); // Secrease
		if(sig == BUTTON_ENTER) stateProgram = PRG_STATE_SETUP_RAMBLOW_MODE;
		if(sig == BUTTON_ESC) stateProgram = PRG_STATE_SETUP_SAVE_PARAMETERS;
		break;
		
		case PRG_STATE_SETUP_RAMBLOW_MODE:
		if(sig == BUTTON_UP) ramBlowActive=1;
		if(sig == BUTTON_DOWN) ramBlowActive=0;
		if(ramBlowActive == ACTIVE && sig == BUTTON_ENTER) stateProgram = PRG_STATE_SETUP_RAMBLOW_TIME;
		if(ramBlowActive == INACTIVE && sig == BUTTON_ENTER) stateProgram = PRG_STATE_SETUP_DELAY_GATES_TIME;
		if(sig == BUTTON_ESC) stateProgram = PRG_STATE_SETUP_DELAY_GATES_TIME; // got to next
		break;
		
		
		case PRG_STATE_SETUP_RAMBLOW_TIME:
		if(sig == BUTTON_UP) setTimeRamBlow(1);
		if(sig == BUTTON_DOWN) setTimeRamBlow(0); // DECREASE
		if(sig == BUTTON_ENTER) stateProgram = PRG_STATE_SETUP_DELAY_GATES_TIME;
		if(sig == BUTTON_ESC) stateProgram = PRG_STATE_SETUP_SAVE_PARAMETERS;
		break;
		
		
		case PRG_STATE_SETUP_DELAY_GATES_TIME:
		if(sig == BUTTON_UP) setTimeDelayGates(1); // INCREASE
		if(sig == BUTTON_DOWN) setTimeDelayGates(0); // decrease
		if(sig == BUTTON_ENTER) stateProgram = PRG_STATE_SETUP_WORKING_TIME;
		if(sig == BUTTON_ESC) stateProgram = PRG_STATE_SETUP_SAVE_PARAMETERS;
		break;
		
		
		case PRG_STATE_SETUP_WORKING_TIME:
		if(sig == BUTTON_UP) setTimeWorking(1); // INCREASE
		if(sig == BUTTON_DOWN)setTimeWorking(0); // decrease
		if(sig == BUTTON_ENTER) stateProgram = PRG_STATE_SETUP_WAITNG_TIME;
		if(sig == BUTTON_ESC) stateProgram = PRG_STATE_SETUP_SAVE_PARAMETERS;
		break;
		
		
		
		case PRG_STATE_SETUP_WAITNG_TIME:
		if(sig == BUTTON_UP) setTimeWating(1); // INCREASE
		if(sig == BUTTON_DOWN) setTimeWating(0); // decrease
		if(sig == BUTTON_ENTER) stateProgram = PRG_STATE_SETUP_HOLD_TIME;
		if(sig == BUTTON_ESC) stateProgram = PRG_STATE_SETUP_SAVE_PARAMETERS;
		break;
		
		case PRG_STATE_SETUP_HOLD_TIME:
		if(sig == BUTTON_UP) setTimeHold(1); // INCREASE
		if(sig == BUTTON_DOWN) setTimeHold(0); // decrease
		if(sig == BUTTON_ENTER) stateProgram = PRG_STATE_SETUP_PHOTOCELL_MODE;
		if(sig == BUTTON_ESC) stateProgram = PRG_STATE_SETUP_SAVE_PARAMETERS;
		break;
		
		case PRG_STATE_SETUP_PHOTOCELL_MODE:
		if(sig == BUTTON_UP) cellActive =1;
		if(sig == BUTTON_DOWN) cellActive =0;
		if(sig == BUTTON_ENTER) stateProgram = PRG_STATE_SETUP_AUTOMATIC_MODE;
		if(sig == BUTTON_ESC) stateProgram = PRG_STATE_SETUP_SAVE_PARAMETERS;
		break;
		
		case PRG_STATE_SETUP_AUTOMATIC_MODE:
		if(sig == BUTTON_UP) autoCloseActive=1;
		if(sig == BUTTON_DOWN) autoCloseActive=0;
		if(sig == BUTTON_ENTER) stateProgram = PRG_STATE_SETUP_STEP_MODE;
		if(sig == BUTTON_ESC) stateProgram = PRG_STATE_SETUP_SAVE_PARAMETERS;
		break;
		
		case PRG_STATE_SETUP_STEP_MODE:
		if(sig == BUTTON_UP) stepActive =1;
		if(sig == BUTTON_DOWN) stepActive=0;
		if(sig == BUTTON_ENTER) stateProgram = PRG_STATE_SETUP_SAVE_PARAMETERS;
		if(sig == BUTTON_ESC) stateProgram = PRG_STATE_SETUP_SAVE_PARAMETERS;
		break;
		
		case PRG_STATE_SETUP_SAVE_PARAMETERS:
		flagSaveMemory++;
		stateProgram = PRG_STATE_RUN;
		break;
		
		default:
		stateProgram = PRG_STATE_RUN;
		break;
		
	}
	
	
}

