#include "main.h"
#include "FreeRTOS.h"
#include "encoder.h"
#include "task.h"


//MSG ID FOR ENCODER SENSOR 0x020
static uint32_t AD_buff [4] ;
 uint8_t length[4] ;

static CAN_HandleTypeDef * GLOB_hcan= NULL;


void Travel_Task_Init(CAN_HandleTypeDef* Copy_hcan)
{

GLOB_hcan=Copy_hcan;

}
void travel_length_data_Task (void*pv)
	{


		while(1)
		{	
			
			
			 
		Travel_length(&length [0]);		
							taskYIELD();

			
		}
		
		
	}