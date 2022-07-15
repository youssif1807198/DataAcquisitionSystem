#ifndef DATA_LOG
#define DATA_LOG

#include "main.h"

typedef struct
{
	uint32_t TimeStamp;
	float ImuAngles[3];
	float LinAcc[3];
	float WheelsTravel[4];	
	double Longitude;
	double latitude;
	float ChasisSpeed;
	float AvrageSpeed;
	float ThrottlePosition;
	uint16_t EngineRPM;
	uint16_t WheelsRPM[4];
	uint8_t EngineECT;
	uint8_t BrakeSwitch;
	int8_t SteeringAngle;

}DAQStrcut;

void DataLogging_Init(void);
void DataLogging_LogDAQ(DAQStrcut* DAQ);

#endif /* DATA_LOG */