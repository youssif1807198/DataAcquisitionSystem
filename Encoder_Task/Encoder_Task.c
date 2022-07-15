#include "main.h"
#include "FreeRTOS.h"
#include "encoder.h"
#include "task.h"


//MSG ID FOR ENCODER SENSOR 0x64A

uint8_t  Message_ENCODER_AND_PROXIMITY_ANGLE [4] ;



static CAN_HandleTypeDef * GLOB_hcan= NULL;
	
int16_t angle = 0 ;

void Encoder_Task_Init(CAN_HandleTypeDef* Copy_hcan)
{

GLOB_hcan=Copy_hcan;

}
			

void enc_angle_data_Task (void*pv)
	{

	
		while(1)
		{
			enc_button_reset();

			angle =  Encoder_angle();

			
			
			
			angle =  Encoder_angle();
			
 
			

	
				taskYIELD();

						
			//while(HAL_CAN_IsTxMessagePending(&hcan,mailbox));	
			
		}
		
	}
			



