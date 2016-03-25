/*
 * main.c
 *
 * Created: 08.03.2016 18:23:18
 *  Author: HSO
 */ 

#define F_CPU 16000000UL



#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#include <inttypes.h>

#include "libs/motors/a498x.h"
//#include "libs/motors/drv881x.h"
#include "libs/adc/adc_analog.h"
#include "libs/usart/usart.h"
#include "libs/timer/timer_utils.h"
#include "libs/utils/my_utils.h"
#include "libs/utils/bit_tools.h"
#include "libs/lcds/hd774x.h"


// Local Stuff
#include "memory.h"
#include "lcdmessages.h"
#include "statemachine.h"
#include "iodefines.h"
#include "power.h"
#include "memory.h"



/************************************************************************/
/* Prototypes                                                                     */
/************************************************************************/
//prototypes
void configGPIO(void);
void schedulerInit(void);




/************************************************************************/
/* Globals Variables                                                                     */
/************************************************************************/
volatile uint8_t flagTaskUsartMessage;

/************************************************************************/
/* Vars modes state                                                                     */
/************************************************************************/
volatile uint8_t preFlashingActive = ACTIVE;
volatile uint8_t ramBlowActive = ACTIVE;
volatile uint8_t stepActive = ACTIVE;
volatile uint8_t cellActive = ACTIVE;
volatile uint8_t autoCloseActive =ACTIVE;

/************************************************************************/
/* Sensors state                                                                     */
/************************************************************************/
volatile uint8_t photoCellsState=0;
volatile uint8_t fca1State=0;
volatile uint8_t fcc1State =0;
volatile uint8_t fca2State =0;
volatile uint8_t fcc2State =0;
volatile uint8_t stopState =0;
volatile uint8_t aux1State =0;
volatile uint8_t aux2State =0;


/************************************************************************/
/* Vars times state                                                                     */
/************************************************************************/
volatile uint16_t preFlashingTime = 2;
volatile uint16_t ramBlowTime =2;
volatile uint16_t delayGatesTime =5; // seconds
volatile uint16_t workingTime =25;
volatile uint16_t waitingTime =10;
volatile uint16_t swingGatesTime =5;
volatile uint16_t unlockHoldTime = 2;
volatile uint16_t holdingTime = 2;


volatile uint16_t powerInitial=1000;
volatile uint16_t powerCourse= 500;

/************************************************************************/
/* State machine                                                                     */
/************************************************************************/
volatile uint8_t stateSwing = STATE_CLOSED_IDLE; // default value
volatile uint8_t flashingActive=0;
volatile uint8_t stateProgram = PRG_STATE_RUN; // DEFAULT
volatile uint8_t flagSaveMemory;





/************************************************************************/
/* SCHEDULER ZONE                                                                   */
/************************************************************************/
// Timer setup for control loop
// freq = (FCPU /prescaler) /timerscale
// timerscale timer0 8bits = 256
//
//#define TIMER0_TICK 0.001
#define TIMER0_SCHED_PRESC TIMER0_PRESC128
#define TIMER0_SCHED_RELOAD 125//// 125 timer inc=1ms

//#define SAMPLING_PERIOD 0.002 // 1ms base time
volatile uint16_t schedulerMaxCount=5000;

//Global Vars
volatile uint8_t flagTaskReadButtons = 0;
volatile uint8_t flagTaskReadSensors = 0;
volatile uint8_t flagTaskStateUpdate=0;
volatile uint8_t flagTaskLcd=0;
volatile uint8_t flagTaskUpdateOutputs =0;
volatile uint8_t flagTaskBlinkLights=0;

// Task periodicity 1ms base tick
volatile uint16_t taskReadButtonsPeriod = 100; // 100ms
volatile uint16_t taskReadSensorsPeriod = 500;
volatile uint16_t taskStateUpdatePeriod =1000; // 1 seconds
volatile uint16_t taskLcdPeriod =500; // 1/5 seconds
volatile uint16_t taskUpdateOutputPeriod =100;
volatile uint16_t taskBlinkLightsPeriod = 1500;

/************************************************************************/
/* @Scheduler Init                                                                     */
/************************************************************************/
void schedulerInit(void){
	
	TCCR0 |= TIMER0_SCHED_PRESC;
	TCCR0 |= TIMER0_WAVEFORM_MODE_CTC;
	OCR0  = TIMER0_SCHED_RELOAD; // timer count reload
	TIMSK |= (1<< OCIE0); // Enable timer compare interrupt
	
}



/************************************************************************/
/* @Config GPIO                                                                     */
/************************************************************************/
void configGPIO(void){
	
	
	DDRE = 0x00; // inputs
	PORTE =0xFF; // enable pullups
	
	DDRF =0xFF; // all outputs
	PORTF=0x00;
	
	DDRA = 0xD0; // low nible input
	PORTA = 0x3F; // enable pullpups
	
}


/////////////////////////////////////////////////////////////
////////////////// GIO ZONE /////////////////////////////////
/////////////////////////////////////////////////////////////
/************************************************************************/
/* @read the buttons and complement and clean                                                                     */
/************************************************************************/
uint8_t readButtons(void){

	return (~PINA) &0x3F; // read the low nibble
}


/************************************************************************/
/* @decode Button                                                                     */
/************************************************************************/
uint8_t decodeButton(uint8_t button){
	button &=0x3F; // ensure
	switch (button){
		
		case 1: return BUTTON_ESC;
		break;
		
		case 2: return BUTTON_ENTER;
		break;
		
		case 4: return BUTTON_UP;
		break;
		
		case 8: return BUTTON_DOWN;
		break;
		
		case 16: return SIGNAL_REMOTE_PED;
		break;
		
		case 32: return SIGNAL_REMOTE_SWING;
		break;
		
		case 3: return BUTTON_PRG; // entering program
		break;
		
		default: return 0;
		break;
	}
}



/************************************************************************/
/* NUMBER CYCLES DEBOUNCE                                                                     */
/************************************************************************/
#define N_DEBOUNCE 3
/************************************************************************/
/* @debounce function                                                                     */
/************************************************************************/
uint8_t debounceKey(uint8_t codeNew){
	uint8_t key =0; // by default
	static uint8_t codeOld;
	static uint8_t keyCount;
	
	// ALREADY SOMETHIN PRESSED
	if(keyCount != 0){
		
		// IF SAME KEY and inside debounce times save
		if(codeNew == codeOld && keyCount <N_DEBOUNCE){ // ONLY IF EQUAL AND DEBOUNCE AVAILABLE
			codeOld =codeNew;
			keyCount++;
			// Reached debounce value and valid key
			if (keyCount == N_DEBOUNCE){
				key = codeNew; // ONLY HERE key is changed;
				
			}
		}
		
	}

	
	// INITIAL CONDITION
	if (keyCount == 0){
		codeOld = codeNew;
		keyCount++;
	}
	
	// if pressed key different reset (user must release the key for new run)
	if(codeNew != codeOld){
		codeOld =codeNew;
		keyCount =1;
	}
	return key;
}


/************************************************************************/
/* SENSOR PIN DEFINITION                                                                     */
/************************************************************************/
#define SENSORS_PORT PINE
#define PIN_SENSOR_CELL 0
#define PIN_SENSOR_FCA1 1
#define PIN_SENSOR_FCC1 2
#define PIN_SENSOR_FCA2 3
#define PIN_SENSOR_FCC2 4
#define PIN_SENSOR_STOP 5
#define PIN_SENSOR_AUX1 6
#define PIN_SENSOR_AUX2 7
/************************************************************************/
/* @ read sensors and activate sensor flags vars                                                                     */
/************************************************************************/
int readSensors(void){

	photoCellsState = bit_read(SENSORS_PORT,PIN_SENSOR_CELL);
	fca1State = bit_read(SENSORS_PORT,PIN_SENSOR_FCA1);
	fcc1State = bit_read(SENSORS_PORT,PIN_SENSOR_FCC1);
	
	fca2State = bit_read(SENSORS_PORT,PIN_SENSOR_FCA2);
	fcc2State = bit_read(SENSORS_PORT,PIN_SENSOR_FCC2);
	
	stopState = bit_read(SENSORS_PORT,PIN_SENSOR_STOP);
	aux1State = bit_read(SENSORS_PORT,PIN_SENSOR_AUX1);
	aux2State = bit_read(SENSORS_PORT,PIN_SENSOR_AUX2);
	
	
	return (1);	
}





////////////////////////////////////////MAIN////////////////////////////////////
int main(void){
	
	USART1_config(USART1_MY_UBBRN,USART_DATA_FORMAT_8BITS|USART_STOP_BITS_1,USART_TRANSMIT_ENABLE|USART_RECEIVE_ENABLE| USART_INTERRUPT_ENABLE);
	//Config IO Ports
	configGPIO();
	//Init Scheduler
	schedulerInit();
	configPWM();
	//Init ADC
	//ADC_init(ADC_ENABLE,ADC_REF_VCC,ADC_MAX_FREQ); //CHECK PARAMS TO FILL IN
	//Init LCD
	LCD_init();
	LCD_clr();
	//LCD_sendString(&temp);
	
	paramLoadFromEeprom();
	
	showLcdSplash(); // show splash
	
	
	//enable interrupts
	sei();
	
	USART1_sendStr("HELLO\n\r");
	
	uint8_t code=0;
	uint8_t portVal=0;
	uint8_t codeSaved=0;
    while(1){
        //TODO:: Please write your application code 
		
		// read inputs
		if(flagTaskReadButtons){
			
			
			
			// Read Buttons
			portVal = readButtons();
			code = decodeButton(portVal);
			code = debounceKey(code);
			
			/*
			#ifdef DEBUG_MAIN
			sprintf(debugBuffer,"Key %d\n\r",code);
			USART1_sendStr(debugBuffer);
			#endif
			*/
			stateMachineProgram(code);
			if(code !=0x00){
				codeSaved = code;
				
			}
			
			
			
			
			flagTaskReadButtons=0;
		}
		
		// read sensors
		if(flagTaskReadSensors){
			//USART1_sendStr("HI\n\r");
			// read sensors
			readSensors();
			flagTaskReadSensors=0;
		}
		
		
		// machine state // chan this to the read
		if(flagTaskStateUpdate){
			
			stateMachineSwing(codeSaved);
			codeSaved =0x00;
			
			flagTaskStateUpdate=0;
		}
		
		
		// Update outpputs
		if(flagTaskUpdateOutputs){
			decodeStateMachine(stateSwing);
			flagTaskUpdateOutputs=0;
		}
		
		
		if(flagTaskBlinkLights){
			
			if(flashingActive){
				// toogle output
				bit_toggle(RELAY_PORT,PIN_LAMP);
				
			}else{
				// clear output
				bit_clear(RELAY_PORT,PIN_LAMP);
			}
			
			flagTaskBlinkLights=0;
		}
		
		
		// lcd stuff
		if(flagTaskLcd){
			
			showLcdMessages(stateSwing);
			
			lcdShowProgram(stateProgram);
				
			
			flagTaskLcd=0;
		}
		
		if(flagSaveMemory){
			paramSavetoEeprom();
			flagSaveMemory=0;
		}
		
		
		
    }
}