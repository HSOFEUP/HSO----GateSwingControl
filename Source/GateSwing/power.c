/*
 * power.c
 *
 * Created: 18.03.2016 19:37:46
 *  Author: HSO
 */ 


#include <avr/io.h>
#include "power.h"



extern volatile uint16_t powerInitial;
extern volatile uint16_t powerCourse;


/************************************************************************/
/* PWM AREA                                                                     */
/************************************************************************/

/************************************************************************/
/* @config PWM                                                                     */
/************************************************************************/
void configPWM(void){
	
	//The formula for Fast PWM
	//F(PWM) = F(Clock)/(N*(1+TOP)
	// max 245 Hz at 16mhz with full top
	
	// fast pwm, mode 14, noninverted output, prescaler 1024 //32
	TCCR1A = (1<<WGM11)|(1<<COM1A1)|(1<<COM1B1);
	//TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10)|(1<<CS12);
	//TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS01) |  (1<<CS00);
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS00);
	
	//ICR1 = (F_CPU[MHz] * periode[us] / prescaler) - 1
	ICR1   = 2560;           // periode cca 0.5s / 8 MHz
	
	DDRB   = (1<<5)|(1<<6);  // output pins OC1A, OC1B   Mega16,32
	
	OCR1A = 0;          // duty cycle = 20 percent
	OCR1B = 0;         // duty cycle = 80 percent
	
	/*
	TCCR1B =0;
	TCCR1A =0;
	TCCR1A |= (1<<COM1B1) |(1<<WGM11);
	TCCR1B |= (1<<WGM13) | (1<<CS10); // CHECK!
	
	ICR1 = TOP_PWM;
	*/
}


/************************************************************************/
/* @ set dimmer to motor pwm led                                                                     */
/************************************************************************/
void setMotorPower(uint16_t m1,uint16_t m2){

	OCR1A =m1;
	OCR1B= m2;
	
}


/************************************************************************/
/* @ set initial power                                                                    */
/************************************************************************/
void setPWMInitial(uint8_t keyUp){
	
	if(keyUp){
		powerInitial += POWER_INCREASE_STEP;
		if(powerInitial >POWER_MAX_VAL) powerInitial = POWER_MAX_VAL;
		}else{
		powerInitial -=POWER_INCREASE_STEP;
		if(powerInitial <POWER_MIN_VAL) powerInitial = POWER_MIN_VAL;
	}
}


/************************************************************************/
/* @ set course power                                                                     */
/************************************************************************/
void setPWMCourse(uint8_t keyUp){
	
	if(keyUp){
		powerCourse += POWER_INCREASE_STEP;
		if(powerCourse >POWER_MAX_VAL) powerCourse = POWER_MAX_VAL;
		}else{
		powerCourse -=POWER_INCREASE_STEP;
		if(powerCourse <POWER_MIN_VAL) powerCourse = POWER_MIN_VAL;
	}
}


