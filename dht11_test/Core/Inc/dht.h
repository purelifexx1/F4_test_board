/*
 * dht.h
 *
 *  Created on: Apr 12, 2021
 *      Author: Admin
 */

#ifndef INC_DHT_H_
#define INC_DHT_H_

#include "main.h"
#include "delay.h"

#define DHT11_GPIO_Port GPIOA
#define DHT11_Pin GPIO_PIN_2


void Set_DHT_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Set_DHT_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

uint8_t DHT11_Read(uint8_t* temp, uint8_t* humi){
	uint8_t dht_data[5] = {0};
	uint64_t startTick;
	// Gui tin hieu request data sang DHT11
	Set_DHT_Pin_Output(DHT11_GPIO_Port, DHT11_Pin);
	HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, 0);
	Delay_ms(20);
	HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, 1);
	Delay(40);
	// Chuyen sang che do input de nhan data
	Set_DHT_Pin_Input(DHT11_GPIO_Port, DHT11_Pin);
	// Chan data keo xuong thap trong 80us
	startTick = get_micros();
	while(!HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin)){
		if((uint64_t)(get_micros() - startTick) > 1000){
			return 255; // timeout
		}
	}
	// Chan data keo len cao trong 80us
	startTick = get_micros();
	while(HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin)){
		if((uint64_t)(get_micros() - startTick) > 100){
			return 255; // timeout
		}
	}
	// Doc 40 bit tra ve
	for(uint8_t i = 0; i < 5; i++){
		for(uint8_t j = 0; j < 8; j++){
			// Chuan bi gui data: muc thap trong 50us
			startTick = get_micros();
			while(!HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin)){
				if((uint64_t)(get_micros() - startTick) > 60){
					return 255; // timeout
				}
			}
			// HIGH for 26-28us = bit 0 / 70us = bit 1
			startTick = get_micros();
			while(HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin)){
				if((uint64_t)(get_micros() - startTick) > 100){
					return 255; // timeout
				}
			}
			if((uint64_t)(get_micros() - startTick) > 40){
				dht_data[i] |= (uint8_t) 1 << (7 - j);
			}
		}
	}
	temp[0] = dht_data[2];
	temp[1] = dht_data[3];
	humi[0] = dht_data[0];
	humi[1] = dht_data[1];
	if(dht_data[0] + dht_data[1] + dht_data[2] + dht_data[3] != dht_data[4]){
		return 0; // checksum error
	}
	return 1;
}

#endif /* INC_DHT_H_ */
