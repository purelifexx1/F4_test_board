/*
 * schedule.cpp
 *
 *  Created on: Mar 31, 2021
 *      Author: Admin
 */

#include "schedule.h"

schedule::schedule(uint32_t Timer)
{
  T = Timer;
}
void schedule::set_cycle(uint32_t Timer)
{
	T = Timer;
}
void schedule::start_timer()
{
  t = HAL_GetTick();
  enable = true;
}
void schedule::stop_timer()
{
  enable = false;
}
void schedule::set_callback(Callback)
{
  this->callback_function = callback_function;
}

void schedule::looping()
{
  if((uint32_t)(HAL_GetTick() - t) >= T && enable == true) {
    t = HAL_GetTick();
    this->callback_function();
  }
}



