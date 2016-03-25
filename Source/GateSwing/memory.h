/*
 * memory.h
 *
 * Created: 16.03.2016 18:34:41
 *  Author: HSO
 */ 


#ifndef MEMORY_H_
#define MEMORY_H_






/************************************************************************/
/* EEPROM AREA                                                                     */
/************************************************************************/
// pid variables
typedef struct{
	uint8_t initEeprom;
	// Modes
	uint8_t preFlashingActive ;
	uint8_t ramBlowActive;
	uint8_t autoCloseActive;
	uint8_t stepActive;
	uint8_t cellActive;
	
	
	// TIMES
	uint16_t preFlashingTime;
	uint16_t ramBlowTime;
	uint16_t unlockHoldTime;
	uint16_t workingTime;
	uint16_t waitingTime;
	uint16_t holdingTime;
	uint16_t delayGatesTime;
	
	uint16_t powerCourse;
	uint16_t powerInitial;
	
	
}eestruct_t;





// TIMES
#define PRE_FLASH_STATE_DEFAULT 1
#define RAM_STATUS_DEFAULT 0
#define AUTO_CLOSE_STATUS_DEFAULT 0
#define STEP_MODE_STATUS_DEFAULT 1
#define CELL_STATUS_DEFAULT 0
// TIMES
#define PRE_FLASH_TIME_DEFAULT 4
#define RAM_TIME_DEFAULT 1
#define UNLOCK_TIME_DEFAULT 3
#define WORKING_TIME_DEFAULT 30
#define WAITING_TIME_DEFAULT 50
#define HOLD_TIME_DEFAULT 3
#define DELAY_GATES_TIME_DEFAULT 10

#define POWER_INITIAL_DEFAULT 1000
#define POWER_COURSE_DEFAULT 500



extern void paramLoadDefaultParameters(void);
extern void eepromSetDefaultParameters(void);
extern void paramLoadFromEeprom(void);
extern void paramSavetoEeprom(void);

#endif /* MEMORY_H_ */