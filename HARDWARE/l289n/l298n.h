#ifndef __L298N_H_
#define __L298N_H_

#include "sys.h"
#include "AllHead.h"

#define MONITORL_ERROR    0

//the right motor
#define Mr_P PGout(4)
#define Mr_N PGout(5)
//the left motor
#define Ml_P PGout(6)
#define Ml_N PGout(7)

void Motor_Init(void);
void Motor_Stop(void);
void Motor_Forward(void);
void Motor_Backward(void);
void Motor_Turnleft(void);
void Motor_Turnright(void);
void left_add(int add);
void right_add(int add);
void Motor_startL(void);
void motor_test(void);
void Motor_Leftback(void);
void Motor_Rightback(void);

#endif
