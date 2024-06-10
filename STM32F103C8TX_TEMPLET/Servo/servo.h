#ifndef __SERVO_H__
#define __SERVO_H__

#include "main.h"
#include "tim.h"


void servo_start(uint8_t id);
void servo_ctrl(int16_t angle);

#endif