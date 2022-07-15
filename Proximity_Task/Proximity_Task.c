/*************************************************************************/  
/*************************************************************************/
/***************  Proximity Task       ***********************************/
/*************** Author:Youssif Khaled ***********************************/
/**************  Layer:HAL             ***********************************/
/**************  Version:1.0           ***********************************/
/**************  Date:27/2/2022       ***********************************/
/*************************************************************************/  
/*************************************************************************/ 

/*CONNECT Channel_1 OF TIMER TO LEFT_FRONT_WHEEL  PROXIMITY SENSOR
& CONNECT Channel_2 OF TIMER TO RIGHT_FRONT_WHEEL PROXIMITY SENSOR
& CONNECT Channel_3 OF TIMER TO LEFT_REAR_WHEEL   PROXIMITY SENSOR
& CONNECT Channel_4 OF TIMER TO RIGHT_REAR_WHEEL  PROXIMITY SENSOR
*/

/*

0x00000002 MSG ID FOR AVERAGE SPEED BY PROXIMITY SPEED
0x00000003 MSG ID FOR RPM OF FOUR WHEELS OF THE CAR 

BYTE_1 & BYTE_2 OF RPM_MSG FOR LEFT_FRONT_WHEEL 
BYTE_3 & BYTE_4 OF RPM_MSG FOR RIGHT_FRONT_WHEEL 
BYTE_5 & BYTE_6 OF RPM_MSG FOR LEFT_REAR_WHEEL 
BYTE_7 & BYTE_8 OF RPM_MSG FOR RIGHT_REAR_WHEEL 



*/

#include "main.h"
#include "Proximity_Task.h"
#include "FreeRTOS.h"
#include "task.h"



extern TaskHandle_t Proximity_Task2_Handle;

static TIM_HandleTypeDef* GLOB_htim=NULL;
static CAN_HandleTypeDef * GLOB_hcan= NULL;



uint16_t AVERAGE_SPEED=0;
uint8_t  WHEELS_RPM [8]={0};
		


	uint16_t frequency_Channel_1=0;
	uint16_t frequency_Channel_2=0;
	uint16_t frequency_Channel_3=0;
	uint16_t frequency_Channel_4=0;

/*VARIABLES FOR CHANNEL_1 
	TO CALCULATE FREQUENCY OF SIGNAL CAPTURED BY CHANNEL_1 */	
	uint32_t IC_Value1_Channel_1=0;
	uint32_t IC_Value2_Channel_1=0;
	uint32_t Difference_Channel_1=0;
	
	
	/*VARIABLES FOR CHANNEL_2 
	TO CALCULATE FREQUENCY OF SIGNAL CAPTURED BY CHANNEL_1 */	
	uint32_t IC_Value1_Channel_2=0;
	uint32_t IC_Value2_Channel_2=0;
	uint32_t Difference_Channel_2=0;
	
	/*VARIABLES FOR CHANNEL_3 
	TO CALCULATE FREQUENCY OF SIGNAL CAPTURED BY CHANNEL_1 */	
	uint32_t IC_Value1_Channel_3=0;
	uint32_t IC_Value2_Channel_3=0;
	uint32_t Difference_Channel_3=0;

	/*VARIABLES FOR CHANNEL_4 
	TO CALCULATE FREQUENCY OF SIGNAL CAPTURED BY CHANNEL_1 */	
	uint32_t IC_Value1_Channel_4=0;
	uint32_t IC_Value2_Channel_4=0;
	uint32_t Difference_Channel_4=0;



	uint8_t Is_Captured_Channel_1=0;

	uint8_t Is_Captured_Channel_2=0;

	uint8_t Is_Captured_Channel_3=0;

	uint8_t Is_Captured_Channel_4=0;
		


void PROXIMITY_Task_INIT(TIM_HandleTypeDef *COPY_htim,CAN_HandleTypeDef* Copy_hcan)
{
	
	
	/*ASSIGN TIMER POINTER TO THE GLOBAL TASK TIMER POINTER
	AND ASSIGN CAN POINTER TO THE GLOBAL TASK CAN POINTER*/
	GLOB_htim=COPY_htim;
	GLOB_hcan=Copy_hcan;
	

	
	
}
		
	
	
	
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	
		/*VALUE OF CLK AFTER DIVIDED BY PRESCALER VALUE*/
		long long Prescaled_Frequency = ((HAL_RCC_GetPCLK2Freq()*4)/12000); 
		
	
	
	    /* xHigherPriorityTaskWoken must be initialised to pdFALSE.  If calling
    xTaskNotifyFromISR() unblocks the handling task, and the priority of
    the handling task is higher than the priority of the currently running task,
    then xHigherPriorityTaskWoken will automatically get set to pdTRUE. */
	
	   BaseType_t  xHigherPriorityTaskWoken;
	
		 xHigherPriorityTaskWoken= pdFALSE;

		xTaskNotifyFromISR(Proximity_Task2_Handle,0,eNoAction,&xHigherPriorityTaskWoken);


	
	/*ISR FOR INPUT CAPTURE UNIT CHANNEL ONE*/
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
			{
		
			if(Is_Captured_Channel_1==0)
			{
				IC_Value1_Channel_1=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
				Is_Captured_Channel_1=1;
			}
			 
			else if(Is_Captured_Channel_1==1)
			{
				IC_Value2_Channel_1=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
				
					if(IC_Value2_Channel_1>IC_Value1_Channel_1)
					{
					Difference_Channel_1=IC_Value2_Channel_1-IC_Value1_Channel_1;
					}
					
					else if(IC_Value2_Channel_1<IC_Value1_Channel_1)
					{
					Difference_Channel_1=( (AutoReloadRegValue-IC_Value1_Channel_1)+IC_Value2_Channel_1 ) ;
					}
					
					else
					{
					Error_Handler();
					}
					
				frequency_Channel_1= (	Prescaled_Frequency / Difference_Channel_1	)/4;
				Is_Captured_Channel_1=0;
				
				/* Force a context switch if xHigherPriorityTaskWoken is now set to pdTRUE.
			The macro used to do this is dependent on the port and may be called
			portEND_SWITCHING_ISR. */
			
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);			
				
			}
			
		}
		/*ISR FOR INPUT CAPTURE UNIT CHANNEL TWO*/

		 if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		 {
				if(Is_Captured_Channel_2==0)
				{
						IC_Value1_Channel_2=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
						Is_Captured_Channel_2=1;
				}
				else if(Is_Captured_Channel_2==1)
				{
						IC_Value2_Channel_2=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
			
					if(IC_Value2_Channel_2>IC_Value1_Channel_2)
					{
						Difference_Channel_2=IC_Value2_Channel_2-IC_Value1_Channel_2;
					}
			
					else if(IC_Value2_Channel_2<IC_Value1_Channel_2)
					{
						Difference_Channel_2=( (AutoReloadRegValue-IC_Value1_Channel_2)+IC_Value2_Channel_2 ) ;
					}
					
					else
					{
						Error_Handler();
					}
					
				frequency_Channel_2= (	Prescaled_Frequency / Difference_Channel_2	)/4;
				Is_Captured_Channel_2=0;
				
				/* Force a context switch if xHigherPriorityTaskWoken is now set to pdTRUE.
			The macro used to do this is dependent on the port and may be called
			portEND_SWITCHING_ISR. */
			
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);				
			
		}
		
	}
			/*ISR FOR INPUT CAPTURE UNIT CHANNEL THREE*/

		 if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		 {
			 
			if(Is_Captured_Channel_3==0)
			{
				IC_Value1_Channel_3=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_3);
				Is_Captured_Channel_3=1;
			}
			else if(Is_Captured_Channel_3==1)
			{
				IC_Value2_Channel_3=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_3);
			
				if(IC_Value2_Channel_3>IC_Value1_Channel_3)
				{
					Difference_Channel_3=IC_Value2_Channel_3-IC_Value1_Channel_3;
				}
			
				else if(IC_Value2_Channel_3<IC_Value1_Channel_3)
				{
					Difference_Channel_3=( (AutoReloadRegValue-IC_Value1_Channel_3)+IC_Value2_Channel_3 ) ;
				}
				else
				{
					Error_Handler();
				}
				
			frequency_Channel_3= (	Prescaled_Frequency / Difference_Channel_3	)/4;
			Is_Captured_Channel_3=0;
			
			/* Force a context switch if xHigherPriorityTaskWoken is now set to pdTRUE.
    The macro used to do this is dependent on the port and may be called
    portEND_SWITCHING_ISR. */
    
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);				
			
		}
	}
		
	
				/*ISR FOR INPUT CAPTURE UNIT CHANNEL FOUR*/
		 if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
		 {
			if(Is_Captured_Channel_4==0)
			{
				IC_Value1_Channel_4=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_4);
				Is_Captured_Channel_4=1;
			}
		 else if(Is_Captured_Channel_4==1)
			{
				IC_Value2_Channel_4=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_4);
			
				if(IC_Value2_Channel_4>IC_Value1_Channel_4)
				{
					Difference_Channel_4=IC_Value2_Channel_4-IC_Value1_Channel_4;			
				}
			
				else if(IC_Value2_Channel_4<IC_Value1_Channel_4)
				{
					Difference_Channel_4=( (AutoReloadRegValue-IC_Value1_Channel_4)+IC_Value2_Channel_4 ) ;
				}
				else
				{
					Error_Handler();
				}
				
			frequency_Channel_4=  (	Prescaled_Frequency / Difference_Channel_4	)/4;
			Is_Captured_Channel_4=0;
				
			/* Force a context switch if xHigherPriorityTaskWoken is now set to pdTRUE.
    The macro used to do this is dependent on the port and may be called
    portEND_SWITCHING_ISR. */
    
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);				
			
		}
		
	}
		
}
	
	
	


void Calculate_Car_Velocity(uint16_t Copy_frequency,uint16_t * Copy_Speed)
{
	/*CALCULATING SPEED USING LAW V=Wr */
	float LOC_Speed = 2*PI*Copy_frequency*Car_Radius ;
	
	/*CONVERTING SPEED INTO KM/HR*/
	LOC_Speed*=ToKMPerHR; 
	
	*Copy_Speed=(uint16_t) LOC_Speed;
	
}



void PROXIMITY_Task(void * Parameters)
{
	
	
		uint16_t speed_LEFT_FRONT_WHEEL =0;
		uint16_t speed_RIGHT_FRONT_WHEEL =0;
		

	
	
	/*STARTING TIMERS*/
	HAL_TIM_IC_Start_IT(GLOB_htim,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(GLOB_htim,TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(GLOB_htim,TIM_CHANNEL_3);
	HAL_TIM_IC_Start_IT(GLOB_htim,TIM_CHANNEL_4);
	

	while(1)
	{
		
		
	
		
		
		/* Block indefinitely (without a timeout, so no need to check the function's
       return value) to wait for a notification (Wait for 0th Notificaition ).*/
		
        xTaskNotifyWait(        0x00,               /* Don't clear any bits on entry. */
                                0x00,               /* Don't Clear all bits on exit. */
                                NULL, /* Receives the notification value. */
                                portMAX_DELAY );    /* Block indefinitely. */

		
		/*CALCULATING SPEED FROM SIGNAL CAPTURED BY PROXIMITY */
		Calculate_Car_Velocity(frequency_Channel_1,&speed_LEFT_FRONT_WHEEL);
		Calculate_Car_Velocity(frequency_Channel_2,&speed_LEFT_FRONT_WHEEL);
		
		/*CALCULTING SPEED FROM FRONT WHEELS OF THE CAR*/
		AVERAGE_SPEED=(speed_LEFT_FRONT_WHEEL+speed_RIGHT_FRONT_WHEEL)/2;
		

		/*frequency multplied by 60 to CONVERT it to RPM */
			
		WHEELS_RPM[0]=(uint8_t)( (frequency_Channel_1*60)>> 8 );
		WHEELS_RPM[1]=(uint8_t)    (frequency_Channel_1*60);
		
		WHEELS_RPM[2]=(uint8_t)( (frequency_Channel_2  *60)  >> 8 );
		WHEELS_RPM[3]=(uint8_t)    (frequency_Channel_2*60);
		
		WHEELS_RPM[4]=(uint8_t)( (frequency_Channel_3*60)>> 8 );
		WHEELS_RPM[5]=(uint8_t)   ( frequency_Channel_3*60);
		
		WHEELS_RPM[6]=(uint8_t)( (frequency_Channel_4*60)>> 8 );
		WHEELS_RPM[7]=(uint8_t)    (frequency_Channel_4*60);
			
			
			
		

		
		
		
		
				taskYIELD();
	
		
	}


	
	
}