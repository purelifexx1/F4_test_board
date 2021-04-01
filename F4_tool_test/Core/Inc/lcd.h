/*
 * lcd.h
 *
 *  Created on: Mar 31, 2021
 *      Author: Admin
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_
#include "main.h"

class lcd{
private:

public:
	lcd();
	void lcd_init (void);   // initialize lcd

	void lcd_send_cmd (char cmd);  // send command to the lcd

	void lcd_send_data (char data);  // send data to the lcd

	void lcd_send_string (char *str);  // send string to the lcd

	void lcd_clear_display (void);	//clear display lcd

	void lcd_goto_XY (int row, int col); //set proper location on screen

};

extern lcd LCD;

#endif /* INC_LCD_H_ */
