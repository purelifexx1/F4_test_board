/*
 * main.cpp
 *
 *  Created on: Mar 30, 2021
 *      Author: Admin
 */

#include "main.h"
#include "schedule.h"
#include "common_define.h"
#include "lcd.h"
#include "math.h"
#include "string.h"
#include "stdlib.h"
#include <vector>
schedule task1(300); //display
schedule task2(200); //button
schedule task3(250); //mode handler
Mode_Status_TypeDef          current_test_mode = MODE_GPIO;
Mode_Status_TypeDef			 selected_mode	   = MODE_GPIO;
GPIO_Mode_Test 		         GPIO_current_test_mode = MODE_1;
uint8_t 					 send[6];
uint8_t 					 receive[6];
typedef struct{
	uint16_t accx;
	uint16_t accy;
	uint16_t accz;
}Acc_TypeDef;
Acc_TypeDef convert;
int count = 0;
uint32_t adc_value;
double step_value = 0;
double accX, accY, accZ;
bool state_value = false;
uint8_t convert_number[20];
char *Mode_Status_String[7] = {
		"GPIO"  ,
		"PWM"   ,
		"UART"  ,
		"I2C"   ,
		"SPI"   ,
		"ADC"   ,
		"USB"
};
char test_string[] = "I2C test ok     ";
std::vector<char> moving_string;

void setup(){
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
	task1.set_callback(task1_handler);
	task2.set_callback(task2_handler);
	task3.set_callback(task3_handler);
	task1.start_timer();
	task2.start_timer();
	task3.start_timer();
	HAL_ADC_Start(&hadc1);
	send[0] = 0x20;
	send[1] = 0x67;
	SPI_WRITE(send, 2, 50);
	delay(10);
	LCD.lcd_init();
	for(int i = 0; i < strlen(test_string); i++){
		moving_string.push_back(test_string[i]);
	}
	moving_string.push_back(0);
}

void loop(){
	task1.looping();
	task2.looping();
	task3.looping();


}

void task1_handler(){ //display task
    LCD.lcd_clear_display();
    delay(5);
    LCD.lcd_goto_XY(2, 0);
    delay(5);
    LCD.lcd_send_string("Mode:");
    LCD.lcd_send_string(Mode_Status_String[current_test_mode]);
	switch(current_test_mode){
		case MODE_GPIO :
			if(GPIO_current_test_mode == MODE_1){
				task3.set_cycle(250);
				LCD.lcd_send_data('1');
			}else if(GPIO_current_test_mode == MODE_2){
				task3.set_cycle(500);
				LCD.lcd_send_data('2');
			}
		break;
		case MODE_PWM  :
			task3.set_cycle(1);
		break;
		case MODE_UART :{
			task3.set_cycle(1000);
			state_value = true;
		}
		break;
		case MODE_I2C  :{
			task3.set_cycle(200);
			LCD.lcd_goto_XY(1, 0);
			delay(2);
			char *temper = &moving_string[0];
			LCD.lcd_send_string(temper);
		}
		break;
		case MODE_SPI  :{
			task3.set_cycle(100);
			LCD.lcd_send_string(" Z:");
			d2s(accZ, convert_number);
			LCD.lcd_send_string((char*)convert_number);
			delay(2);
			LCD.lcd_goto_XY(1, 0);
			delay(2);
			LCD.lcd_send_string("X:");
			d2s(accX, convert_number);
			LCD.lcd_send_string((char*)convert_number);
			LCD.lcd_send_string(" Y:");
			d2s(accY, convert_number);
			LCD.lcd_send_string((char*)convert_number);

		}
		break;
		case MODE_ADC  :{
			task3.set_cycle(100);
			LCD.lcd_goto_XY(1, 0);
			delay(2);
			LCD.lcd_send_string("Vtage value:");
			double voltage = adc_value/4096.0*3.0;
			d2s(voltage, convert_number);
			LCD.lcd_send_string((char*)convert_number);
		}
	    break;
		case MODE_USB  :{
			task3.set_cycle(1000);
			state_value = true;
		}
		break;

		default		   :
		break;
	}

}
void task3_handler(){
	switch(current_test_mode){
		case MODE_GPIO :
			if(GPIO_current_test_mode == MODE_1){
				ALL_LED_SET(0);
				switch(count){
				case 0:
					PD12_SET(1024);
					break;
				case 1:
					PD13_SET(1024);
					break;
				case 2:
					PD14_SET(1024);
					break;
				case 3:
					PD15_SET(1024);
					break;
				}
				count = (count+1)%4;
			}else if(GPIO_current_test_mode == MODE_2){
				switch(count){
				case 0:
					ALL_LED_SET(0);
					break;
				case 1:
					ALL_LED_SET(1024);
					break;
				}
				count = (count+1)%2;
			}
		break;
		case MODE_PWM  :{
			ALL_LED_SET(count);
			count = 512 + (int)(512*sin(2*3.14159*0.5*step_value));
			step_value += 0.001;
		}
		break;
		case MODE_UART :{
			if(state_value == true){
				UART_WRITE((uint8_t*)"Testing OK\r\n", sizeof("Testing OK\r\n")-1);
			}
		}
		break;
		case MODE_I2C  :{
			char temper_value = moving_string.at(0);
			moving_string.erase(moving_string.begin());
			moving_string[moving_string.size() - 1] = temper_value;
			moving_string.push_back(0);
		}
		break;
		case MODE_SPI  :{
			send[0] = 0x28 | 0x80;
			send[1] = 0x29 | 0x80;
			send[2] = 0x2a | 0x80;
			send[3] = 0x2b | 0x80;
			send[4] = 0x2c | 0x80;
			send[5] = 0x2d | 0x80;
			SPI_RECEIVE(send, receive, 6, 50);
			int temper = receive[0] << 8 | receive[1];
			if(temper >= 0 && temper <= 32767) {
				accZ = -temper*2.0/32767;
			}else if(temper <= 65535 && temper >=32768){
				accZ = 2.0 - (temper-32767)/16384.0;
			}

			temper = receive[2] << 8 | receive[3];
			if(temper >= 0 && temper <= 32767) {
				accY = -temper*2.0/32767;
			}else if(temper <= 65535 && temper >=32768){
				accY = 2.0 - (temper-32767)/16384.0;
			}

			temper = receive[4] << 8 | receive[5];
			if(temper >= 0 && temper <= 32767) {
				accX = -temper*2.0/32767;
			}else if(temper <= 65535 && temper >=32768){
				accX = 2.0 - (temper-32767)/16384.0;
			}
		}
		break;
		case MODE_ADC  :{
			adc_value = HAL_ADC_GetValue(&hadc1);
		}
		break;
		case MODE_USB  :{
			if(state_value == true){
				//USB_WRITE((uint8_t*)"Testing OK\r\n", sizeof("Testing OK\r\n")-1);
			}
		}
		break;

		default		   :
		break;
	}
}

void task2_handler(){ //button handler
	if(READ_INC_BT == 1){
		count = 0;step_value = 0;state_value = false;
		current_test_mode = (Mode_Status_TypeDef)((current_test_mode + 1)%7);
	}
	if(READ_DEC_BT == 1){
		count = 0;step_value = 0;state_value = false;
		current_test_mode = (Mode_Status_TypeDef)((current_test_mode == 0)?MODE_USB:(current_test_mode - 1));
	}

	if(READ_USER_BT == 1){
		GPIO_current_test_mode = (GPIO_current_test_mode == MODE_1)?MODE_2:MODE_1;
		count = 0;
	}

}

void d2s(double number, uint8_t *output){ //number smaller than 10
	if(number > 0){
		output[0] = '0' + (int)(number);
		output[1] = '.';
		output[2] = '0' + ((int)(number*10))%10;
		output[3] = '0' + ((int)(number*100))%10;
		output[4] = 0;
	}else{
		double number1 = fabs(number);
		output[0] = '-';
		output[1] = '0' + (int)(number1);
		output[2] = '.';
		output[3] = '0' + ((int)(number1*10))%10;
		output[4] = '0' + ((int)(number1*100))%10;
		output[5] = 0;
	}

}

