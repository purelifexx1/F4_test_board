/*
 * common_define.h
 *
 *  Created on: Mar 30, 2021
 *      Author: Admin
 */

#ifndef INC_COMMON_DEFINE_H_
#define INC_COMMON_DEFINE_H_

#include "main.h"
#define delay(ms)											     HAL_Delay(ms)
#define PD12_SET(value)	                                         (TIM4->CCR1 = value)
#define PD13_SET(value)                                          (TIM4->CCR2 = value)
#define PD14_SET(value)                                          (TIM4->CCR3 = value)
#define PD15_SET(value)                                          (TIM4->CCR4 = value)
#define CS_LOW													 HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET)
#define CS_HIGH													 HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET)
#define ALL_LED_SET(value)									     {           \
																 PD12_SET(value);\
																 PD13_SET(value);\
																 PD14_SET(value);\
																 PD15_SET(value);\
																 }
#define READ_INC_BT											     HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_8)
#define READ_DEC_BT											     HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_10)
//#define READ_ENTER_BT											 HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_10)
#define READ_USER_BT										     HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)

#define UART_WRITE(content, size)							     HAL_UART_Transmit_DMA(&huart2, content, size)
#define USB_WRITE(content, size)								 CDC_Transmit_FS(content, size)
#define SPI_WRITE(send, size, timeout)				 			 {                                                \
															     CS_LOW;                                          \
																 HAL_SPI_Transmit(&hspi1, send, size, timeout);   \
																 CS_HIGH;										  \
																 }
#define SPI_RECEIVE(send, receive, size, timeout)				 {                                                \
															     CS_LOW;                                          \
																 HAL_SPI_Transmit(&hspi1, send, size, timeout);   \
																 HAL_SPI_Receive(&hspi1, receive, size, timeout); \
																 CS_HIGH;										  \
																 }

typedef enum {
	MODE_GPIO ,
	MODE_PWM  ,
	MODE_UART ,
	MODE_I2C  ,
	MODE_SPI  ,
	MODE_ADC  ,
	MODE_USB  ,
}Mode_Status_TypeDef;

typedef enum{
	MODE_1  ,
	MODE_2  ,
}GPIO_Mode_Test;





#endif /* INC_COMMON_DEFINE_H_ */
