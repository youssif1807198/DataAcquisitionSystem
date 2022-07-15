#ifndef _ENCODER_TASK_H_
#define _ENCODER_TASK_H_

#include "main.h"


void Encoder_Task_Init(CAN_HandleTypeDef* Copy_hcan);

void enc_angle_data_Task (void*pv);



#endif