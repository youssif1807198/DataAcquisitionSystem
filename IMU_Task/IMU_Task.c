/*************************************************************************/  
/*************************************************************************/
/***************        IMU TASK       ***********************************/
/*************** Author:Youssif Khaled ***********************************/
/**************  Layer:HAL             ***********************************/
/**************  Version:1.0           ***********************************/
/**************  Date:22/2/2022       ***********************************/
/*************************************************************************/  
/*************************************************************************/  


//MSG ID 0x00000000 FOR EULER ANGLES OF IMU
//MSG ID 0x00000001 FOR 3D ACCLERATION ANGLES OF IMU

#include "BNO055_Interface.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "BNO055_Private.h"


uint32_t pervious,current,time;


	/*ARRAY OF FLOAT TO STORE READING FROM IMU AFTER PROCESSING ON IT TO CALCULATE EULER ANGLES*/
		float EulerAngles_Float[3]={0};

	/*IN THIS ARRAY EULER ANGLES STORED AFTER MULTIPLYING THEM BY 100 AND DIVIDE THEM INTO TWO BYTES*/
	uint8_t DividedEulerAngles[6]={0};	
	
	
	/*ARRAY OF FLOAT TO STORE ACCLERATION DATA IN 3D FROM IMU REGISTERS*/
	float AcclerationData_Float[3]={0};

	/*IN THIS ARRAY EULER ANGLES STORED AFTER MULTIPLYING THEM BY 100 AND DIVIDE THEM INTO TWO BYTES*/
	uint8_t DividedAcclerationData[6]={0};		
	


static CAN_HandleTypeDef * GLOB_hcan= NULL;


void IMU_Task_INIT(I2C_HandleTypeDef*  Copy_hi2c,CAN_HandleTypeDef* Copy_hcan)
{
	
	GLOB_hcan=Copy_hcan;
	
	BNO055_Handler_Init(Copy_hi2c);
	BNO055_Begin_uint8_t(NDOF);
	BNO055_SetExternalCrytal_Void();
	
	
}




void IMU_Task(void * Parameters)
{
		/*IN THIS ARRAY EULER ANGLES STORED AFTER MULTIPLYING THEM BY 100*/
	int16_t EulerAnglesTimes100[3]={0};
	
		/*IN THIS ARRAY EULER ANGLES STORED AFTER MULTIPLYING THEM BY 100*/
	int16_t AcclerationDataTimes100[3]={0};
	


	



	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

		
	while (1)
  {
		
		
//		 pervious=HAL_GetTick();
		
		BNO055_EulerFromQuaternion(EulerAngles_Float);
		BNO055_GetThreeAxesData(ACCELEROMETER_DATA,AcclerationData_Float);
		

	
		
		/*MUTIPLY EULER ANGLES BY 100 (TWO DECIMAL VALUES ARE TAKEN)TO CONVERT THEM INTO INTEGER VALUE 
		TO BE TRANSMITTED THROUGH CAN BUS */
		EulerAnglesTimes100[0]=EulerAngles_Float[0] *100; //REPRESENTS ROLL ANGLE
		EulerAnglesTimes100[1]=EulerAngles_Float[1] *100; //REPRESENTS PITCH ANGLE  
		EulerAnglesTimes100[2]=EulerAngles_Float[2] *100; //REPRESENTS HEADING ANGLE 
		
		
		
		/*MUTIPLY ACCELEROMETER DATA BY 100 (TWO DECIMAL VALUES ARE TAKEN)TO CONVERT THEM INTO INTEGER VALUE 
		TO BE TRANSMITTED THROUGH CAN BUS */
		AcclerationDataTimes100[0]=AcclerationData_Float[0] *100; //REPRESENTS ACCLERATION IN X-AXIS
		AcclerationDataTimes100[1]=AcclerationData_Float[1] *100; //REPRESENTS ACCLERATION IN Y-AXIS
		AcclerationDataTimes100[2]=AcclerationData_Float[2] *100; //REPRESENTS ACCLERATION IN Z-AXIS
		
	
		taskENTER_CRITICAL();
		uint8_t Iterator_U8=0;
		for(Iterator_U8=0;Iterator_U8<5;Iterator_U8 += 2)
		{
			/*DIVIDE EACH EULER ANGLE AFTER MULTIPLYING IT BY 100 INTO
		TWO BYTES TO FIT INTO CAN TRANSMIT FUNCTION*/
		DividedEulerAngles[Iterator_U8]  =(uint8_t)  EulerAnglesTimes100[Iterator_U8/2];
		DividedEulerAngles[Iterator_U8+1]=(uint8_t)  ( ( EulerAnglesTimes100[Iterator_U8/2] )>>8 );
		
		/*DIVIDE EACH ACCLERATION IN EACH DIRECTION AFTER MULTIPLYING IT BY 100 INTO
		TWO BYTES TO FIT INTO CAN TRANSMIT FUNCTION*/
		DividedAcclerationData[Iterator_U8]  =(uint8_t)  AcclerationDataTimes100[Iterator_U8/2];
		DividedAcclerationData[Iterator_U8+1]=(uint8_t)( AcclerationDataTimes100[Iterator_U8/2]>>8 );
		
		}
		taskEXIT_CRITICAL();
		

//		current=HAL_GetTick();
//		
//		time=current-pervious;
		
		taskYIELD();
		
//	vTaskDelay(pdMS_TO_TICKS(50) );
		
		
		// Wait for the next cycle.
    //vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS(50) );
		
		
		

		
		
		
		

  }
	
	
	
	
	
	
	
	
	
	
	
}
