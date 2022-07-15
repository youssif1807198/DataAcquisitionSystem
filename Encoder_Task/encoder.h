#ifndef __ENCODER_H
#define __ENCODER_H
#include <stm32f4xx_hal.h>
#include <stdint.h>
#include <stdbool.h>



#ifndef ENCODER_TIM
#define ENCODER_TIM                            htim2
#endif

/*max angle before the timer auto reset*/
#define MAX_ANGLE 180


/*Number of pulses per one revolution "dependes on the encoder"*/
#define PPR  360 

/*Travel sensor Max Length in mm*/
#define TML 100 

extern TIM_HandleTypeDef htim2;
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;


void TIM_ResetCounter(TIM_TypeDef* TIMx);
void enc_button_reset(void);
void Encoder_Init(void);
void Travel_Init(void);
int16_t Encoder_angle(void);
uint16_t Encoder_raw(void);
void Travel_length(uint8_t travels[4]);
void Travel_Task_Init(CAN_HandleTypeDef* Copy_hcan);
void Encoder_Task_Init(CAN_HandleTypeDef* Copy_hcan);




#endif