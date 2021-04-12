/*
 * delay.h
 *
 *  Created on: Apr 12, 2021
 *      Author: Admin
 */

#ifndef INC_DELAY_H_
#define INC_DELAY_H_

#include "main.h"

#define XTAL 100
#define au32_ticks	(SystemCoreClock / 1000000)

void DWT_Init(void){
	if(!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)){
		CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
		DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
	}
	DWT->CYCCNT = 0;
}

void Delay(uint32_t us){
	uint32_t au32_initial_ticks = DWT->CYCCNT;
	us *= au32_ticks;
	while ((DWT->CYCCNT - au32_initial_ticks) < us-au32_ticks);
}
void Delay_ms(uint32_t ms){
	while(ms--) Delay(1000);
}

uint64_t get_micros(void){
	return DWT->CYCCNT / au32_ticks;
}
uint32_t get_millis(void){
	return DWT->CYCCNT / (au32_ticks * 1000);
}

#endif /* INC_DELAY_H_ */
