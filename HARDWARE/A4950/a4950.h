#ifndef __A4950_H_
#define __A4950_H_

#include "sys.h"
#include "AllHead.h"

#define MAX_ARR     799

void start_forward(void);
void stop_forward(void);
void set_speed(int speed_L,int speed_R);
void MotorA_start(void);
void motorA_test(void);
void turn_left_A(void);
void turn_right_A(void);

#endif
