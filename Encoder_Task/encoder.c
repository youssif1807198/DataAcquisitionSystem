#include "main.h"

#include "encoder.h"
/*buffer for the DMA of the ADC */
 uint32_t AD_buff[4] ;



/*Stores the raw data of the encoder*/
int16_t  counter = 0;

/*Stores the angle data of the encoder*/
int16_t static position = 0;

/*Stores the status of the reset button*/
bool static  button_status = 1 ;

void Encoder_Init(void)
{
	/*initalizing the encoder interface on TIM2*/
  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	
}
void Travel_Init(void)
{
	/*Calibrates the adc*/
	//HAL_ADCEx_Calibration_Start(&hadc1);
	/*Start the DMA of the adc*/
	HAL_ADC_Start_DMA(&hadc1, &AD_buff, 4);
}

uint16_t Encoder_raw()
{
	/*getting the raw data of the encoder*/
	counter = __HAL_TIM_GET_COUNTER(&htim2);
	return counter  ;
}

int16_t Encoder_angle()
{
	/*type casting the data so that 65534 >> -1*/
	counter =(int16_t) __HAL_TIM_GET_COUNTER(&htim2);
	
	/*calculating the exact angle by knowing how many pulses per revolution*/
	position = (counter)*360/PPR;
	
	/*checking if the angle is a possible value and reseting it if otherwise*/
	if ( (position >= MAX_ANGLE) || (position <= (-1*MAX_ANGLE)))
		{
			TIM_ResetCounter(TIM2);
			position =0 ;
		}
	return position  ;
}
void enc_button_reset (void)
{
	 /*checking the status of the reset button*/
//	 button_status = HAL_GPIO_ReadPin( GPIOC , GPIO_PIN_14 );
//	
//	if( button_status == 0 )
//	{
//		TIM_ResetCounter(TIM4);
//		position = 0 ;
//	}
}
void TIM_ResetCounter(TIM_TypeDef* TIMx)
{
	
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
	
	/*setting the counter register to zero to reset*/
  TIMx->CNT = 0; //to be tested
	
}
void Travel_length(uint8_t *travels)
{
		
			int8_t i = 0 ;
			/*a loop to map the values of the ADC into length and stores it*/
	
			for ( i = 0; i < 4;	i++)
			{
				travels[i] = (uint8_t) (AD_buff[i]/(4096/TML));
			}
	
	
}
