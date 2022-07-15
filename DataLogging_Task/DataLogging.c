#include "DataLogging.h"

#include "fatfs.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"

#define DATA_LOG_NOT_INITIALIZED 0
#define DATA_LOG_INITIALIZED		 1

FATFS 	myFATAFS;
FIL 		myFILE;
UINT 		testByte;
FRESULT Status;
FILINFO fno;

uint8_t DataLoggingInitState = DATA_LOG_NOT_INITIALIZED;
uint8_t SD_CardInstalled = 0;
float Prv , Current;
float Time;
char DaqBuffer[200],TimeStamp[15], ACCx[6] , ACCy[6] , ACCz[6], Roll[6], Pitch[6], Yaw[6];
char WheelsRPM[4][5],WheelsTravel[4][6], ChasisSpeed[6] , AvgSpeed[6] , EngineRPM[5], ThrottlePosition[6], EngineECT[3];
char longitude[10],latitude[10],BrakeSwitch[5],SteeringAngle[6], Tab[] = "\t" , NewLine[] = "\n";

int count;
char FileName [15] = "LOG1.TXT\0";
extern SD_HandleTypeDef hsd;

void DataLogging_Init(void)
{
	uint32_t DataLoggingInitTimeOut = 0;
	uint32_t TickStart = 0;
	uint8_t  FileNumber = 0;
	char NewFileNumber[5];

	TickStart =  HAL_GetTick();
	HAL_SD_DeInit(&hsd);
	HAL_SD_Init(&hsd);
	FATFS_UnLinkDriver(SDPath);
  FATFS_LinkDriver(&SD_Driver, SDPath);
	Status = f_mount(&myFATAFS, SDPath, 1);

	while(f_mount (&myFATAFS, SDPath, 1) != FR_OK)
	{
			if(HAL_GetTick() - TickStart >= 10)
			{
					DataLoggingInitState = DATA_LOG_NOT_INITIALIZED;
				  return;
			}
	}
	if(SD_CardInstalled == 0)
	{
		while(f_stat(FileName,&fno) == FR_OK)
		{
			FileNumber = FileName[3] - '0';
			
			if((FileNumber == 9) && (!isdigit(FileName[4])))
			{
				strcpy(FileName , "LOG10.TXT\0");
			}
			else if(isdigit(FileName[4]))
			{
					FileNumber = ((FileName[3] - '0')*10 + (FileName[4] - '0'))+1  ;
					snprintf(NewFileNumber,3,"%d",FileNumber);
					FileName[3] = NewFileNumber[0]; 
					FileName[4] = NewFileNumber[1]; 
			}
			else
			{
				FileNumber++;
				FileName[3] = FileNumber + '0';
			}
			
		}
	
	
		f_open(&myFILE,FileName,FA_WRITE|FA_OPEN_ALWAYS|FA_READ);
		f_puts("Time Stamp\tAccX\tAccY\tAccZ\tAngleX\tAngleY\tAngleZ\tChasisSpeed\tAvgSpeed\tEngineRPM\tEngineECT\tThrottlePosition\tBrakeSwitch\tSteeringAngle\tWS_FL\tWS_FR\tWS_RL\tWS_RR\tTravel_FL\tTravel_FR\tTravel_RL\tTravel_RR\tlongitude\tlatitude\n", &myFILE);
		f_close(&myFILE);
		SD_CardInstalled = 1;
	}
	DataLoggingInitState = DATA_LOG_INITIALIZED;
}

void DataLogging_LogDAQ(DAQStrcut* DAQ)
{
 if(DataLoggingInitState == DATA_LOG_INITIALIZED)
 {
		Prv = HAL_GetTick();
		//DAQ->TimeStamp = HAL_GetTick();
		snprintf(TimeStamp,15,"%d",count);
		snprintf(ACCx,6,"%f",DAQ->LinAcc[0]);
		snprintf(ACCy,6,"%f",DAQ->LinAcc[1]);
		snprintf(ACCz,6,"%f",DAQ->LinAcc[2]);
		snprintf(Roll,6,"%f",DAQ->ImuAngles[0]);
		snprintf(Pitch,6,"%f",DAQ->ImuAngles[1]);
		snprintf(Yaw,6,"%f",DAQ->ImuAngles[2]);
		snprintf(WheelsRPM[0],6,"%d",DAQ->WheelsRPM[0]);
		snprintf(WheelsRPM[1],6,"%d",DAQ->WheelsRPM[1]);
		snprintf(WheelsRPM[2],6,"%d",DAQ->WheelsRPM[2]);
		snprintf(WheelsRPM[3],6,"%d",DAQ->WheelsRPM[3]);
		snprintf(WheelsTravel[0],5,"%f",DAQ->WheelsTravel[0]);
		snprintf(WheelsTravel[1],5,"%f",DAQ->WheelsTravel[1]);
		snprintf(WheelsTravel[2],5,"%f",DAQ->WheelsTravel[2]);
		snprintf(WheelsTravel[3],5,"%f",DAQ->WheelsTravel[3]);
		snprintf(ChasisSpeed,6,"%f",DAQ->ChasisSpeed);
		snprintf(AvgSpeed,6,"%f",DAQ->AvrageSpeed);
		snprintf(EngineRPM,5,"%d",DAQ->EngineRPM);
		snprintf(EngineECT,3,"%d",DAQ->EngineECT);
		snprintf(ThrottlePosition,6,"%f",DAQ->ThrottlePosition);
		snprintf(SteeringAngle,6,"%d",DAQ->SteeringAngle);
		snprintf(BrakeSwitch,5,"%d",DAQ->BrakeSwitch);
		snprintf(latitude,10,"%f",DAQ->latitude);
		snprintf(longitude,10,"%f",DAQ->Longitude);
		
		strcat(DaqBuffer,TimeStamp);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,ACCx);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,ACCy);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,ACCz);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,Roll);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,Pitch);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,Yaw);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,ChasisSpeed);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,AvgSpeed);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,EngineRPM);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,EngineECT);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,ThrottlePosition);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,BrakeSwitch);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,SteeringAngle);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,WheelsRPM[0]);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,WheelsRPM[1]);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,WheelsRPM[2]);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,WheelsRPM[3]);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,WheelsTravel[0]);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,WheelsTravel[1]);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,WheelsTravel[2]);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,WheelsTravel[3]);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,longitude);
		strcat(DaqBuffer,Tab);
		strcat(DaqBuffer,latitude);
		strcat(DaqBuffer,NewLine);

	//	while(f_mount (&myFATAFS, SDPath, 1) != FR_OK);

		Status = f_open(&myFILE, FileName,FA_WRITE);
		if(Status != FR_OK)
		{
				DataLoggingInitState = DATA_LOG_NOT_INITIALIZED;
		  	HAL_SD_DeInit(&hsd);
				return;
		}
		f_lseek(&myFILE,f_size(&myFILE));
		f_puts(DaqBuffer,&myFILE);
		while(f_close(&myFILE)!= FR_OK);
		memset(DaqBuffer, 0 , sizeof(DaqBuffer));
		Current = HAL_GetTick();
		Time = (Current - Prv);
		count++;

	}
	else
	{
			DataLogging_Init();
	}
	


}