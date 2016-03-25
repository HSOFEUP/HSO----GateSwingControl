/*
 * statemachine.h
 *
 * Created: 16.03.2016 18:36:06
 *  Author: HSO
 */ 


#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_




/************************************************************************/
/* SWING STATE                                                                     */
/************************************************************************/
#define STATE_CLOSED_IDLE 1

#define STATE_PRE_FLASHING 2

// PEDONAL STATES
#define STATE_UNLOCK 101
#define STATE_OPENING_M1_RAM_PED 102
#define STATE_OPENING_M1_INITIAL_PED 103
#define STATE_OPENING_M1_COURSE_PED 104
#define STATE_WATTING_M1_OPEN_PED 105
#define STATE_OPENING_M1_INITIAL_COURSE 106
#define STATE_WAITING_M1_TIME_OR_SIGNAL_PED 107
#define STATE_CLOSING_M1_RAM_PED 108
#define STATE_CLOSING_M1_INITIAL_PED 109
#define STATE_CLOSING_M1_COURSE_PED 110
#define STATE_HOLD_CLOSING_M1_PED 111
#define STATE_CELL_CLOSING_M1_PED 112
#define STATE_HOLD_OPENING_M1_PED 113


// SWING STATES
#define STATE_OPENING_M1_RAM_M2_HOLD_SWING 201
#define STATE_OPENING_M1_INITIAL_M2_HOLD_SWING 202
#define STATE_OPENING_M1_COURSE_M2_RAM_SWING 203

#define STATE_OPENING_M1_COURSE_M2_HOLD_SWING 204

#define STATE_OPENING_M1_COURSE_M2_INITIAL_SWING 205
#define STATE_OPENING_M1_COURSE_M2_COURSE_SWING 206

#define STATE_WAITING_M1M2_SIGNAL_OR_SIGNAL_SWING 207

#define STATE_CLOSING_M1_HOLD_M2_RAM_SWING 208
#define STATE_CLOSING_M1_HOLD_M2_INITIAL_SWING 209
#define STATE_CLOSING_M1_RAM_M2_COURSE_SWING 210

#define STATE_CLOSING_M1_COURSE_M2_HOLD_SWING 211

#define STATE_CLOSING_M1_INITIAL_M2_COURSE_SWING 212
#define STATE_CLOSING_M1_COURSE_M2_COURSE_SWING 213

#define STATE_HOLD_CLOSING_M1M2_SWING 214
#define STATE_CELL_CLOSING_M1M2_SWING 215
#define STATE_HOLD_OPENING_M1M2_SWING 216



/************************************************************************/
/* PRG STATE                                                                     */
/************************************************************************/
#define PRG_STATE_RUN 1

#define PRG_STATE_SETUP_PREFLASHING_MODE 2
#define PRG_STATE_SETUP_PREFLASHING_TIME 3

#define PRG_STATE_SETUP_UNLOCK_TIME 4

#define PRG_STATE_SETUP_RAMBLOW_MODE 44
#define PRG_STATE_SETUP_RAMBLOW_TIME 5

#define PRG_STATE_SETUP_DELAY_GATES_TIME 6
#define PRG_STATE_SETUP_WORKING_TIME 7
#define PRG_STATE_SETUP_WAITNG_TIME 8

#define PRG_STATE_SETUP_HOLD_TIME 9

#define PRG_STATE_SETUP_PHOTOCELL_MODE 10
#define PRG_STATE_SETUP_AUTOMATIC_MODE 11
#define PRG_STATE_SETUP_STEP_MODE 12
#define PRG_STATE_SETUP_SAVE_PARAMETERS 13

#define PRG_STATE_SETUP_POWER_INI 16
#define PRG_STATE_SETUP_POWER_COU 17


extern int stateMachineSwing(uint8_t sig);
extern int decodeStateMachine(uint8_t state);
extern void stateMachineProgram (uint8_t sig);



#endif /* STATEMACHINE_H_ */