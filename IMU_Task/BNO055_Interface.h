/*************************************************************************/  
/*************************************************************************/
/***************       BNO055 IMU      ***********************************/
/*************** Author:Youssif Khaled ***********************************/
/**************  Layer:HAL             ***********************************/
/**************  Version:1.0           ***********************************/
/**************  Date:30/12/2021       ***********************************/
/*************************************************************************/  
/*************************************************************************/  

#ifndef _BNO055_INTERFACE_H_
#define _BNO055_INTERFACE_H_

#include "main.h"
#include "BNO055_Config.h"


                        /*Power Modes*/
typedef enum{												
 Normal_Mode    = 0,
 Low_Power_Mode = 1,
 Suspend_Mode   = 2
}BNO055_PWR_MODES;
                        /*Opearting Modes*/

typedef enum{
	/*CONFIG-MODE*/
	CONFIG_MODE = 0,

	/*Non-FusionMode*/
	ACCONLY      =    1,
	MAGONLY      =    2,
	GYROONLY     =    3,
	ACCMAG       =    4,
	ACCGYRO      =    5,
	MAGGYRO      =    6,
	AMG          =    7,

	/*Fusion Mode*/
	IMU           =    8, 
	COMPASS       =    9,
	M4G           =    10,
	NDOF_FMC_OFF  =    11,
	NDOF          =    12
}BNO055_OPERATING_MODES; 


typedef enum{

	MAGNETOMETER_DATA   =0,
	GYROSCOPE_DATA      =1,
	EULER_DATA          =2,
	ACCELEROMETER_DATA  =3,
	LINEARACCEL_DATA    =4,
	GRAVITY_DATA        =5

}BNO055_DATA;




void BNO055_Handler_Init(I2C_HandleTypeDef*  Copy_hi2c);
uint8_t BNO055_Begin_uint8_t (BNO055_OPERATING_MODES Copy_Operating_Mode);
void BNO055_SetMode_Void(BNO055_OPERATING_MODES Copy_Operating_Mode) ;
void BNO055_SetExternalCrytal_Void(void);
int8_t BNO055_GetTemp_int8_t (void);
void BNO055_GetThreeAxesData(BNO055_DATA Copy_BNO55_Data,float Copy_XYZ_Data[3]);
void BNO055_GetCalibrationData_Void(uint8_t * Copy_sys, uint8_t * Copy_gyro,uint8_t * Copy_accel, uint8_t * Copy_mag);
void BNO055_GetQuaternionData(float Copy_WXYZ_Data[4]);
void BNO055_EulerFromQuaternion (float Copy_EulerAngles[3]);





#endif
