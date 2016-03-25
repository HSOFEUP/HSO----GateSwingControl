/*
 * power.h
 *
 * Created: 18.03.2016 19:38:33
 *  Author: HSO
 */ 


#ifndef POWER_H_
#define POWER_H_

#define TOP_PWM 1600 // VALOR PWM 16Bits // aqui pode-se ajustar

#define POWER_INCREASE_STEP 10
#define POWER_MAX_VAL 1600
#define POWER_MIN_VAL 100



extern void configPWM(void);
extern void setMotorPower(uint16_t m1,uint16_t m2);
extern void setPWMInitial(uint8_t keyUp);
extern void setPWMCourse(uint8_t keyUp);



#endif /* POWER_H_ */