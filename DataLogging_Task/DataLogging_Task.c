/*************************************************************************/  
/*************************************************************************/
/***************        Data logging TASK       **************************/
/*************** Author:Youssif Khaled ***********************************/
/**************  Layer:HAL             ***********************************/
/**************  Version:1.0           ***********************************/
/**************  Date:25/2/2022        ***********************************/
/*************************************************************************/  
/*************************************************************************/ 


#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "DataLogging_Task.h"

extern float EulerAngles_Float[3];
extern float AcclerationData_Float[3];

extern uint16_t AVERAGE_SPEED;
extern uint8_t length[4] ;
extern int16_t angle ;

extern	uint16_t frequency_Channel_1;
extern	uint16_t frequency_Channel_2;
extern	uint16_t frequency_Channel_3;
extern	uint16_t frequency_Channel_4;



void VehicleLogAllData(void * pvParameters)
{
	
	DAQStrcut DataAcquisition;
	
	
	TickType_t xLastWakeTime;
	vTaskSetApplicationTaskTag(NULL , (void* )1);
	xLastWakeTime = xTaskGetTickCount();
	
	 DataLogging_Init();

	while(1)
	{
		taskENTER_CRITICAL();
		DataAcquisition.ImuAngles[0]= EulerAngles_Float[0];
		DataAcquisition.ImuAngles[1]= EulerAngles_Float[1];
		DataAcquisition.ImuAngles[2]= EulerAngles_Float[2];
		
		
		DataAcquisition.LinAcc[0]=AcclerationData_Float[0];
		DataAcquisition.LinAcc[1]=AcclerationData_Float[1];
		DataAcquisition.LinAcc[2]=AcclerationData_Float[2];


		DataAcquisition.AvrageSpeed = AVERAGE_SPEED;
		DataAcquisition.ChasisSpeed = 41.47;
		DataAcquisition.WheelsRPM[0] =  frequency_Channel_1*60;
		DataAcquisition.WheelsRPM[1] = frequency_Channel_2*60;
		DataAcquisition.WheelsRPM[2] = frequency_Channel_3*60;
		DataAcquisition.WheelsRPM[3] = frequency_Channel_4*60;
		DataAcquisition.WheelsTravel[0] = length[0];
		DataAcquisition.WheelsTravel[1] = length[1];
		DataAcquisition.WheelsTravel[2] = length[2];
		DataAcquisition.WheelsTravel[3] = length[3];
		DataAcquisition.BrakeSwitch = 1;
		DataAcquisition.EngineRPM = 8574;
		DataAcquisition.EngineECT = 78;
		DataAcquisition.latitude = 30.06656583;
		DataAcquisition.Longitude = 31.02895648;
		DataAcquisition.SteeringAngle = angle;
		DataAcquisition.ThrottlePosition = 37.8;
		

DataLogging_LogDAQ(&DataAcquisition);
		taskEXIT_CRITICAL();
		taskYIELD();
	}
	
}

