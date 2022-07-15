/*************************************************************************/  
/*************************************************************************/
/***************        CAN TASK       ***********************************/
/*************** Author:Youssif Khaled ***********************************/
/**************  Layer:MCAL            ***********************************/
/**************  Version:1.0           ***********************************/
/**************  Date:12/5/2022        ***********************************/
/*************************************************************************/  
/*************************************************************************/  

//MSG ID 0x00000000 FOR EULER ANGLES OF IMU
//MSG ID 0x00000001 FOR 3D ACCLERATION ANGLES OF IMU


#include "main.h"
#include "FreeRTOS.h"
#include "task.h"


static CAN_HandleTypeDef * GLOB_hcan= NULL;


extern uint8_t DividedAcclerationData[6];		
extern uint8_t DividedEulerAngles[6];

extern uint8_t  WHEELS_RPM [8];

extern uint8_t  Message_ENCODER_AND_PROXIMITY_ANGLE [4] ;

extern uint16_t AVERAGE_SPEED;
extern int16_t angle ;



		

extern uint8_t length[4] ;






void CAN_Task_INIT(CAN_HandleTypeDef* Copy_hcan)
{
	
		GLOB_hcan=Copy_hcan;

	
}


		
void CAN_Task(void * Parameters)
{


	
	/*TX HEADER CONFIGURATION TO TRANSMIT EULER ANGLES	*/
	
	uint32_t pTxMailbox_IMU_EulerAngles;
	
	CAN_TxHeaderTypeDef pheader_IMU_EulerAngles;
	pheader_IMU_EulerAngles.DLC=                      6;           //6 BYTES FOR EULER ANGLES AS EACH ANGLE TAKES TWO BYTES
	pheader_IMU_EulerAngles.ExtId=                    0;           // No Extension Will Be Used
	pheader_IMU_EulerAngles.IDE=             CAN_ID_STD;
	pheader_IMU_EulerAngles.RTR=           CAN_RTR_DATA;
	pheader_IMU_EulerAngles.StdId=           0x51;           //MSG ID FOR EULER ANGLES OF IMU
	pheader_IMU_EulerAngles.TransmitGlobalTime=DISABLE ;

   /*TX HEADER CONFIGURATION TO TRANSMIT ACCLERATION IN 3D	*/
	
	uint32_t pTxMailbox_IMU_3D_ACC;
	
	CAN_TxHeaderTypeDef pheader_IMU__3D_ACC        ;
	pheader_IMU__3D_ACC.DLC=                      6;           //6 BYTES FOR EULER ANGLES AS EACH ANGLE TAKES TWO BYTES
	pheader_IMU__3D_ACC.ExtId=                    0;           // No Extension Will Be Used
	pheader_IMU__3D_ACC.IDE=             CAN_ID_STD;
	pheader_IMU__3D_ACC.RTR=           CAN_RTR_DATA;
	pheader_IMU__3D_ACC.StdId=           0x52;           //MSG ID FOR 3D ACCLERATION ANGLES OF IMU
	pheader_IMU__3D_ACC.TransmitGlobalTime=DISABLE ;
	
	
	
	/*TX HEADER CONFIGURATION TO TRANSMIT AVERAGE SPEED 	*/
	
	uint32_t pTxMailbox_PROXIMITY_SPEED_AND_ENCODER;
	
	CAN_TxHeaderTypeDef pheader_PROXIMITY_SPEED_AND_ENCODER ;
	pheader_PROXIMITY_SPEED_AND_ENCODER.DLC=                      4;           //TWO BYTES WILL BE TRANSMITTED AS AVERAGE SPEED AND OTHER TWO BYTES FOR STERRING ANGLE
	pheader_PROXIMITY_SPEED_AND_ENCODER.ExtId=                    0;           // No Extension Will Be Used
	pheader_PROXIMITY_SPEED_AND_ENCODER.IDE=             CAN_ID_STD;
	pheader_PROXIMITY_SPEED_AND_ENCODER.RTR=           CAN_RTR_DATA;
	pheader_PROXIMITY_SPEED_AND_ENCODER.StdId=           0x53;           //MSG ID FOR AVERAGE SPEED BY PROXIMITY SPEED
	pheader_PROXIMITY_SPEED_AND_ENCODER.TransmitGlobalTime=DISABLE ;

   /*TX HEADER CONFIGURATION TO TRANSMIT RPM OF WHEELS 	*/
	
	uint32_t pTxMailbox_PROXIMITY_RPM;
	
	CAN_TxHeaderTypeDef pheader_PROXIMITY_RPM        ;
	pheader_PROXIMITY_RPM.DLC=                      8;           //8 BYTES FOR  RPM OF FOUR WHEELS OF THE CAR AS EACH WHEEL RPM  TAKES TWO BYTES
	pheader_PROXIMITY_RPM.ExtId=                    0;           // No Extension Will Be Used
	pheader_PROXIMITY_RPM.IDE=             CAN_ID_STD;
	pheader_PROXIMITY_RPM.RTR=           CAN_RTR_DATA;
	pheader_PROXIMITY_RPM.StdId=           0x54;           //MSG ID FOR RPM OF FOUR WHEELS OF THE CAR;
	pheader_PROXIMITY_RPM.TransmitGlobalTime=DISABLE ;
	
	
	/*TX HEADER CONFIGURATION TO TRANSMIT ANGLE OF ENCODER 	*/

/* // to be deleted 
	uint32_t pTxMailbox_ENCODER;
	
	CAN_TxHeaderTypeDef pheader_ENCODER        ;
	pheader_PROXIMITY_RPM.DLC=                      2;           //2 BYTES FOR ENCODER ANGLE 
	pheader_PROXIMITY_RPM.ExtId=                    0;           // No Extension Will Be Used
	pheader_PROXIMITY_RPM.IDE=             CAN_ID_STD;
	pheader_PROXIMITY_RPM.RTR=           CAN_RTR_DATA;
	pheader_PROXIMITY_RPM.StdId=           0x00000004;           //MSG ID FOR RPM OF FOUR WHEELS OF THE CAR;
	pheader_PROXIMITY_RPM.TransmitGlobalTime=DISABLE ;
	
*/	
		
		

	

	/*TX HEADER CONFIGURATION TO TRANSMIT LENGHTS OF TRAVEL 	*/
	
	uint32_t pTxMailbox_TRAVEL;
	
	CAN_TxHeaderTypeDef pheader_TRAVEL        ;
	pheader_TRAVEL.DLC=                      4;           //4 BYTES FOR TRAVEL SENSORS LENGTHS 
	pheader_TRAVEL.ExtId=                    0;           // No Extension Will Be Used
	pheader_TRAVEL.IDE=             CAN_ID_STD;
	pheader_TRAVEL.RTR=           CAN_RTR_DATA;
	pheader_TRAVEL.StdId=           0x55;           //MSG ID FOR TEAVELS;
	pheader_TRAVEL.TransmitGlobalTime=DISABLE ;
	
	
	HAL_CAN_Start(GLOB_hcan);


		
	while (1)
  {
	  
	
  taskENTER_CRITICAL();
		
	HAL_CAN_AddTxMessage(GLOB_hcan,&pheader_IMU_EulerAngles,&DividedEulerAngles[0],&pTxMailbox_IMU_EulerAngles);
	HAL_CAN_AddTxMessage(GLOB_hcan,&pheader_IMU__3D_ACC,&DividedAcclerationData[0],&pTxMailbox_IMU_3D_ACC);	
	
	HAL_CAN_AddTxMessage(GLOB_hcan,&pheader_PROXIMITY_SPEED_AND_ENCODER,& Message_ENCODER_AND_PROXIMITY_ANGLE [0],&pTxMailbox_PROXIMITY_SPEED_AND_ENCODER);
	HAL_CAN_AddTxMessage(GLOB_hcan,&pheader_PROXIMITY_RPM,&WHEELS_RPM[0],&pTxMailbox_PROXIMITY_RPM);

//	HAL_CAN_AddTxMessage(GLOB_hcan,&pTxMailbox_ENCODER,&message[0],&pTxMailbox_ENCODER);
		
		
		
	Message_ENCODER_AND_PROXIMITY_ANGLE[0] =(uint8_t) (AVERAGE_SPEED);
	Message_ENCODER_AND_PROXIMITY_ANGLE[1] =(uint8_t) (AVERAGE_SPEED>>8); 
	Message_ENCODER_AND_PROXIMITY_ANGLE[2] =(uint8_t) (angle);
	Message_ENCODER_AND_PROXIMITY_ANGLE[3] =(uint8_t) (angle>>8);
	
	HAL_CAN_AddTxMessage(GLOB_hcan,&pheader_TRAVEL,&length[0],&pTxMailbox_TRAVEL);
		
	taskEXIT_CRITICAL();

	
	
	
								taskYIELD();

	

  }
	
	
	
	
	
	
	
	
	
	
	
}		