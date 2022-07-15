/*************************************************************************/  
/*************************************************************************/
/***************       BNO055 IMU      ***********************************/
/*************** Author:Youssif Khaled ***********************************/
/**************  Layer:HAL             ***********************************/
/**************  Version:1.0           ***********************************/
/**************  Date:30/12/2021       ***********************************/
/*************************************************************************/  
/*************************************************************************/  



#include "BNO055_Private.h"
#include "BNO055_Config.h"
#include "main.h"
#include "BNO055_Interface.h"
#include <math.h>

#define PI 3.14159265359

static float LOC_WXYZ_Data[4]={0};
//float LOC_EulerAngles[3]={0};

static  I2C_HandleTypeDef* Glob_hi2c= NULL;


void BNO055_Handler_Init(I2C_HandleTypeDef*  Copy_hi2c)
{

 Glob_hi2c= Copy_hi2c;


}



uint8_t BNO055_Begin_uint8_t (BNO055_OPERATING_MODES Copy_Operating_Mode)
{  
	
	uint8_t LOC_Chip_Id =0;
  uint8_t iterator =0;			

	


	
	HAL_I2C_Mem_Read( Glob_hi2c,BNO055_Address_Primary ,BNO055_CHIP_ID_Address ,1 ,&LOC_Chip_Id ,1 ,HAL_MAX_DELAY);





	
	while(LOC_Chip_Id!=0xA0 && iterator <=2)     
	{
		
		
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
		HAL_Delay(1000);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
		HAL_Delay(1000);
		
		HAL_I2C_DeInit(Glob_hi2c);		
		HAL_Delay(10);
		HAL_I2C_Init(Glob_hi2c);
		HAL_Delay(10);
		
		HAL_I2C_Mem_Read(Glob_hi2c, BNO055_Address_Primary, BNO055_CHIP_ID_Address, 1, &LOC_Chip_Id, 1, HAL_MAX_DELAY);



		
		if(LOC_Chip_Id!=0xA0 && iterator==3	)
		{
			return 0; // return false
		}
		
		iterator ++;

	}
	
	BNO055_SetMode_Void(CONFIG_MODE);
	
	uint8_t SYS_TRIGGER_Config=0x20;/*Set to reset system*/
	HAL_I2C_Mem_Write(Glob_hi2c,BNO055_Address_Primary,SYS_TRIGGER_Address,1,&SYS_TRIGGER_Config,1,HAL_MAX_DELAY);

	HAL_Delay(30);
	
	LOC_Chip_Id=0;	 //rest variable again
	HAL_I2C_Mem_Read(Glob_hi2c,BNO055_Address_Primary,BNO055_CHIP_ID_Address,1,&LOC_Chip_Id,1,HAL_MAX_DELAY);

	 while (LOC_Chip_Id!=0xA0)
	 {
		 	HAL_I2C_Mem_Read(Glob_hi2c,BNO055_Address_Primary,BNO055_CHIP_ID_Address,1,&LOC_Chip_Id,1,HAL_MAX_DELAY);

	 }
	 	HAL_Delay(50);
	 
		uint8_t MODE_CONFIG=Normal_Mode;
	  HAL_I2C_Mem_Write(Glob_hi2c,BNO055_Address_Primary,PWR_MODE_Address,1,&MODE_CONFIG,1,HAL_MAX_DELAY);
		HAL_Delay(10);
		
		
		uint8_t PAGE_ID_Config=0;
	 	HAL_I2C_Mem_Write(Glob_hi2c,BNO055_Address_Primary,PAGE_ID_Address,1,&PAGE_ID_Config,1,HAL_MAX_DELAY);

		
		SYS_TRIGGER_Config=0;	/*disable any rests return to default value*/
	 	HAL_I2C_Mem_Write(Glob_hi2c,BNO055_Address_Primary,SYS_TRIGGER_Address,1,&SYS_TRIGGER_Config,1,HAL_MAX_DELAY);
		HAL_Delay(10);
		
		BNO055_SetMode_Void(Copy_Operating_Mode);
		HAL_Delay(20);

		return 1; //return true


		
	
	
}


void BNO055_SetMode_Void(BNO055_OPERATING_MODES Copy_Operating_Mode) 
{
	

	HAL_I2C_Mem_Write(Glob_hi2c,BNO055_Address_Primary,OPR_MODE_Address,1,&Copy_Operating_Mode,1,HAL_MAX_DELAY);
  HAL_Delay(30);

}






void BNO055_SetExternalCrytal_Void(void)
{

	BNO055_OPERATING_MODES LOC_Previous_Mode;
	HAL_I2C_Mem_Read(Glob_hi2c,BNO055_Address_Primary,OPR_MODE_Address,1,&LOC_Previous_Mode,1,HAL_MAX_DELAY);
	
	BNO055_SetMode_Void(CONFIG_MODE);
	HAL_Delay(25);
	
	uint8_t PAGE_ID_CONFIG = 0;
	HAL_I2C_Mem_Write(Glob_hi2c,BNO055_Address_Primary,PAGE_ID_Address,1,&PAGE_ID_CONFIG,1,HAL_MAX_DELAY);

	uint8_t SYS_TRIGGER_CONFIG=0x80;
	HAL_I2C_Mem_Write(Glob_hi2c,BNO055_Address_Primary,SYS_TRIGGER_Address,1,&SYS_TRIGGER_CONFIG,1,HAL_MAX_DELAY);
	
 	HAL_Delay(10);
	BNO055_SetMode_Void(LOC_Previous_Mode);
	HAL_Delay(20);

}
	



int8_t BNO055_GetTemp_int8_t (void)
{
	int8_t LOC_Temp;
	
	HAL_I2C_Mem_Read(Glob_hi2c,BNO055_Address_Primary,TEMP_Address,1,&LOC_Temp,1,HAL_MAX_DELAY);
	
	return  LOC_Temp ;
}

void BNO055_GetThreeAxesData(BNO055_DATA Copy_BNO55_Data,float Copy_XYZ_Data[3])
{
	
	uint8_t buffer[6]={0};
	 float LOC_XYZ_Data[3]={0};
	
	switch (Copy_BNO55_Data) {
  case MAGNETOMETER_DATA:
    /* 1uT = 16 LSB */
	
	
	/*GETTING MAGNOMETER DATA  */
		HAL_I2C_Mem_Read(Glob_hi2c,BNO055_Address_Primary,BNO055_MAG_DATA_X_LSB_ADDR,1,&buffer[0],6,HAL_MAX_DELAY);

		
	/*ASSIGN LSB AND MSB BITS TO ONE FLOATING POINT VARIABLE*/
    LOC_XYZ_Data[0] = (int16_t) (  ((uint16_t)buffer[0]) | (((uint16_t)buffer[1]) << 8) );
    LOC_XYZ_Data[1] = (int16_t) (  ((uint16_t)buffer[2]) | (((uint16_t)buffer[3]) << 8) );
	  LOC_XYZ_Data[2] = (int16_t) (  ((uint16_t)buffer[4]) | (((uint16_t)buffer[5]) << 8) );
	

    LOC_XYZ_Data[0] = ((double)LOC_XYZ_Data[0]) / 16.0;
    LOC_XYZ_Data[1] = ((double)LOC_XYZ_Data[1]) / 16.0;
    LOC_XYZ_Data[2] = ((double)LOC_XYZ_Data[2]) / 16.0;
    break;
  
	case GYROSCOPE_DATA:
    /* 1dps = 16 LSB */
	
		/*GETTING GYROSCOPE DATA  */

		HAL_I2C_Mem_Read(Glob_hi2c,BNO055_Address_Primary,BNO055_GYRO_DATA_X_LSB_ADDR,1,&buffer[0],6,HAL_MAX_DELAY);

	/*ASSIGN LSB AND MSB BITS TO ONE FLOATING POINT VARIABLE*/
    LOC_XYZ_Data[0] = (int16_t) ( ((uint16_t)buffer[0]) | (((uint16_t)buffer[1]) << 8) );
    LOC_XYZ_Data[1] = (int16_t) ( ((uint16_t)buffer[2]) | (((uint16_t)buffer[3]) << 8) );
	  LOC_XYZ_Data[2] = (int16_t) ( ((uint16_t)buffer[4]) | (((uint16_t)buffer[5]) << 8) );
	

    LOC_XYZ_Data[0] = ((double)LOC_XYZ_Data[0]) / 16.0;
    LOC_XYZ_Data[1] = ((double)LOC_XYZ_Data[1]) / 16.0;
    LOC_XYZ_Data[2] = ((double)LOC_XYZ_Data[2]) / 16.0;
    break;

  case EULER_DATA:
    /* 1 degree = 16 LSB */
	
			/*GETTING EULER DATA  */

	
		HAL_I2C_Mem_Read(Glob_hi2c,BNO055_Address_Primary,BNO055_EULER_H_LSB_ADDR,1,&buffer[0],6,HAL_MAX_DELAY);

		
	/*ASSIGN LSB AND MSB BITS TO ONE doubleING POINT VARIABLE*/
    LOC_XYZ_Data[0] = (int16_t) ( ((uint16_t)buffer[0]) | (((uint16_t)buffer[1]) << 8)  );
    LOC_XYZ_Data[1] = (int16_t) ( ((uint16_t)buffer[2]) | (((uint16_t)buffer[3]) << 8)  );
	  LOC_XYZ_Data[2] = (int16_t) ( ((uint16_t)buffer[4]) | (((uint16_t)buffer[5]) << 8)  );
	

    LOC_XYZ_Data[0] = ((double)LOC_XYZ_Data[0]) / 16.0;
    LOC_XYZ_Data[1] = ((double)LOC_XYZ_Data[1]) / 16.0;
    LOC_XYZ_Data[2] = ((double)LOC_XYZ_Data[2]) / 16.0;
    break;

  case ACCELEROMETER_DATA:
    /* 1m/s^2 = 100 LSB */
	
			/*GETTING ACCELROMOTER DATA  */

		HAL_I2C_Mem_Read(Glob_hi2c,BNO055_Address_Primary,BNO055_ACCEL_DATA_X_LSB_ADDR,1,&buffer[0],6,HAL_MAX_DELAY);

		
	/*ASSIGN LSB AND MSB BITS TO ONE FLOATING POINT VARIABLE*/
    LOC_XYZ_Data[0] = (int16_t)   ( ((uint16_t)buffer[0]) | (((uint16_t)buffer[1]) << 8)   ); 
    LOC_XYZ_Data[1] = (int16_t)   ( ((uint16_t)buffer[2]) | (((uint16_t)buffer[3]) << 8)   );
	  LOC_XYZ_Data[2] = (int16_t)   ( ((uint16_t)buffer[4]) | (((uint16_t)buffer[5]) << 8)   );
	

    LOC_XYZ_Data[0] = ((double)LOC_XYZ_Data[0]) / 100.0;
    LOC_XYZ_Data[1] = ((double)LOC_XYZ_Data[1]) / 100.0;
    LOC_XYZ_Data[2] = ((double)LOC_XYZ_Data[2]) / 100.0;
    break;
 
  case LINEARACCEL_DATA:
    /* 1m/s^2 = 100 LSB */
	
				/*GETTING LINEAR ACCELRATION DATA  */

		HAL_I2C_Mem_Read(Glob_hi2c,BNO055_Address_Primary,BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR,1,&buffer[0],6,HAL_MAX_DELAY);

		
	/*ASSIGN LSB AND MSB BITS TO ONE FLOATING POINT VARIABLE*/
    LOC_XYZ_Data[0] = (int16_t) ( ((uint16_t)buffer[0]) | (((uint16_t)buffer[1]) << 8) );
    LOC_XYZ_Data[1] = (int16_t) ( ((uint16_t)buffer[2]) | (((uint16_t)buffer[3]) << 8) );
	  LOC_XYZ_Data[2] = (int16_t) ( ((uint16_t)buffer[4]) | (((uint16_t)buffer[5]) << 8) );
	

    LOC_XYZ_Data[0] = ((double)LOC_XYZ_Data[0]) / 100.0;
    LOC_XYZ_Data[1] = ((double)LOC_XYZ_Data[1]) / 100.0;
    LOC_XYZ_Data[2] = ((double)LOC_XYZ_Data[2]) / 100.0;
    break;

  case GRAVITY_DATA:
    /* 1m/s^2 = 100 LSB */
	
				/*GETTING GRAVITY DATA  */

		HAL_I2C_Mem_Read(Glob_hi2c,BNO055_Address_Primary,BNO055_GRAVITY_DATA_X_LSB_ADDR,1,&buffer[0],6,HAL_MAX_DELAY);

		
	/*ASSIGN LSB AND MSB BITS TO ONE FLOATING POINT VARIABLE*/
    LOC_XYZ_Data[0] = (int16_t) ( ((uint16_t)buffer[0]) | (((uint16_t)buffer[1]) << 8) );
    LOC_XYZ_Data[1] = (int16_t) ( ((uint16_t)buffer[2]) | (((uint16_t)buffer[3]) << 8) );
	  LOC_XYZ_Data[2] = (int16_t) ( ((uint16_t)buffer[4]) | (((uint16_t)buffer[5]) << 8) );
	

    LOC_XYZ_Data[0] = ((double)LOC_XYZ_Data[0]) / 100.0;
    LOC_XYZ_Data[1] = ((double)LOC_XYZ_Data[1]) / 100.0;
    LOC_XYZ_Data[2] = ((double)LOC_XYZ_Data[2]) / 100.0;
    break;

  }

	/*Assigning values to the given argument array*/
  Copy_XYZ_Data[0] =LOC_XYZ_Data[0];
	Copy_XYZ_Data[1] =LOC_XYZ_Data[1];
	Copy_XYZ_Data[2] =LOC_XYZ_Data[2];
}


																		 
void BNO055_GetCalibrationData_Void(uint8_t * Copy_sys, uint8_t * Copy_gyro,uint8_t * Copy_accel, uint8_t * Copy_mag)
{
		uint8_t LOC_Calibration_Data=0;
		HAL_I2C_Mem_Read(Glob_hi2c ,BNO055_Address_Primary ,CALIB_STAT_Address ,1 ,&LOC_Calibration_Data ,1 ,HAL_MAX_DELAY);
	if (Copy_sys != NULL) {
    *Copy_sys = (LOC_Calibration_Data >> 6) & 0x03;
  }
  if (Copy_gyro != NULL) {
    *Copy_gyro = (LOC_Calibration_Data >> 4) & 0x03;
  }
  if (Copy_accel != NULL) {
    *Copy_accel = (LOC_Calibration_Data >> 2) & 0x03;
  }
  if (Copy_mag != NULL) {
    *Copy_mag = LOC_Calibration_Data & 0x03;
  }


}



 void BNO055_GetQuaternionData(float Copy_WXYZ_Data[4])
{
	uint8_t buffer[8]={0};
	double Loc_Scale= (1.0/ (1<<14) );
	/*
	LOC_WXYZ_Data [0] REPRESENTS W 
	LOC_WXYZ_Data [1] REPRESENTS X
	LOC_WXYZ_Data [2] REPRESENTS Y
	LOC_WXYZ_Data [3] REPRESENTS Z
	
	*/
	
	/*temp varaibles to store u16 values*/
	int16_t Temp_W=0,Temp_X=0,Temp_Y=0,Temp_Z=0;

	HAL_I2C_Mem_Read(Glob_hi2c,BNO055_Address_Primary,BNO055_QUATERNION_DATA_W_LSB_ADDR,1,&buffer[0],8,HAL_MAX_DELAY);

		/*ASSIGN LSB AND MSB BITS TO ONE FLOATING POINT VARIABLE*/
    Temp_W= (  (((uint16_t)buffer[1]) << 8) | ((uint16_t)buffer[0])   );
    Temp_X= (  (((uint16_t)buffer[3]) << 8) | ((uint16_t)buffer[2])   );
	  Temp_Y= (  (((uint16_t)buffer[5]) << 8) | ((uint16_t)buffer[4])   );
		Temp_Z= (  (((uint16_t)buffer[7]) << 8) | ((uint16_t)buffer[6])   );
	
	
	
	
	  LOC_WXYZ_Data[0] = ((float)Temp_W) * Loc_Scale;
    LOC_WXYZ_Data[1] = ((float)Temp_X) * Loc_Scale;
    LOC_WXYZ_Data[2] = ((float)Temp_Y) * Loc_Scale;
    LOC_WXYZ_Data[3] = ((float)Temp_Z) * Loc_Scale;
		
		
		
		/*Assigning values to the given argument array*/
		
		Copy_WXYZ_Data[0]=LOC_WXYZ_Data[0];
		Copy_WXYZ_Data[1]=LOC_WXYZ_Data[1];
		Copy_WXYZ_Data[2]=LOC_WXYZ_Data[2];
		Copy_WXYZ_Data[3]=LOC_WXYZ_Data[3];


  

}



void BNO055_EulerFromQuaternion (float Copy_EulerAngles[3])
{
	 float  LOC_EulerAngles[3]={0};
	 
	 BNO055_GetQuaternionData(LOC_WXYZ_Data);

	/*
	LOC_EulerAngles [0] REPRESENTS ROLL ANGLE 
	LOC_EulerAngles [1] REPRESENTS PITCH ANGLE  
	LOC_EulerAngles [2] REPRESENTS HEADING ANGLE 
	*/
	 
	 
	  double sqw =  LOC_WXYZ_Data[0] * LOC_WXYZ_Data[0];
    double sqx =  LOC_WXYZ_Data[1] * LOC_WXYZ_Data[1];
    double sqy =  LOC_WXYZ_Data[2] * LOC_WXYZ_Data[2];
    double sqz =  LOC_WXYZ_Data[3] * LOC_WXYZ_Data[3];
	 
	 double sinr_0=0,cosr_0=0,sinr_1=0,sinr_2=0,cosr_2=0;
	 
	  cosr_0=2.0f * (LOC_WXYZ_Data[2] * LOC_WXYZ_Data[3] + LOC_WXYZ_Data[1] * LOC_WXYZ_Data[0]) ;
	  sinr_0=(-sqx - sqy + sqz + sqw);
	  LOC_EulerAngles [0] = atan2f(cosr_0,sinr_0 );

    sinr_1=-2.0f * (LOC_WXYZ_Data[1] * LOC_WXYZ_Data[3] - LOC_WXYZ_Data[2] * LOC_WXYZ_Data[0]) / (sqx + sqy + sqz + sqw) ;
    LOC_EulerAngles [1] = asinf(sinr_1);
	 
	  cosr_2=2.0f * (LOC_WXYZ_Data[1] * LOC_WXYZ_Data[2] + LOC_WXYZ_Data[3] * LOC_WXYZ_Data[0]);
    sinr_2=(sqx - sqy - sqz + sqw);	 
    LOC_EulerAngles [2] = atan2f(cosr_2,sinr_2);
	 
	 
	 /*conversion between radian and degrees*/
	 
	 LOC_EulerAngles [0] *=180.0f/PI;
	 LOC_EulerAngles [1] *=180.0f/PI;
	 LOC_EulerAngles [2] *=180.0f/PI;
	 
	 
	 if(LOC_EulerAngles[2]>= +180.0f)
	 {
		 LOC_EulerAngles[2]-=360.0f;
	 
	 }
	 else if(LOC_EulerAngles[2]< -180.0f)
	 {
		 LOC_EulerAngles[2] +=360.0f;
	 }
	 
	 	/*Assigning values to the given argument array*/

	 Copy_EulerAngles[0]=LOC_EulerAngles[0];
	 Copy_EulerAngles[1]=LOC_EulerAngles[1];
	 Copy_EulerAngles[2]=LOC_EulerAngles[2];

	 
	 
	 

}










