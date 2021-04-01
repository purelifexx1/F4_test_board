/*
 * schedule.h
 *
 *  Created on: Mar 31, 2021
 *      Author: Admin
 */

#ifndef INC_SCHEDULE_H_
#define INC_SCHEDULE_H_
#include "main.h"

#define Callback void(*callback_function)()
class schedule
{
  private:
    Callback;
    uint32_t T;
    uint32_t t = 0;
    bool enable = false;
  public:
    schedule(uint32_t Timer);
    void set_cycle(uint32_t Timer);
    void start_timer();
    void stop_timer();
    void set_callback(Callback);
    void looping();
};



#endif /* INC_SCHEDULE_H_ */
