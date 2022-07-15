#ifndef _TRAVEL_TASK_H_
#define _TRAVEL_TASK_H_


#include "main.h"


void Travel_Task_Init(CAN_HandleTypeDef* Copy_hcan);
void enc_angle_data_Task (void*pv);
void travel_length_data_Task (void*pv);

#endif