/*************************************************************************/  
/*************************************************************************/
/***************  Proximity Task       ***********************************/
/*************** Author:Youssif Khaled ***********************************/
/**************  Layer:HAL             ***********************************/
/**************  Version:1.0           ***********************************/
/**************  Date:27/2/2022       ***********************************/
/*************************************************************************/  
/*************************************************************************/  



#ifndef _PROXIMITY_TASK_H_
#define _PROXIMITY_TASK_H_


#include "main.h"


 #define AutoReloadRegValue 6000
 #define PI 3.14159265359f
 #define Car_Radius 0.2286f //RADIUS OF CAR IN METERS
 #define ToKMPerHR 0.2777777778f


void PROXIMITY_Task_INIT(TIM_HandleTypeDef *COPY_htim,CAN_HandleTypeDef* Copy_hcan);
void Calculate_Car_Velocity(uint16_t Copy_frequency,uint16_t * Copy_Speed);
void PROXIMITY_Task(void * Parameters);











#endif