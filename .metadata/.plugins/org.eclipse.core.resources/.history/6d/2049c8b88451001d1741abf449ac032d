/*
 * SleepMilli.cpp
 *
 *  Created on: 14 Oct 2022
 *      Author: Manish
 */

#include "SleepMilli.h"
#include <atomic>
#include "chip.h"

static volatile std::atomic_int counter;

#ifdef __cplusplus
extern "C" {
#endif

void SysTick_Handler(void)
{
	if(counter > 0) counter--;
}

#ifdef __cplusplus
}
#endif


SleepMilli::SleepMilli() {
	// TODO Auto-generated constructor stub

}

void SleepMilli::Sleep(int ms)
{
	counter = ms;
	while(counter > 0) {
		__WFI();
	}
}

SleepMilli::~SleepMilli() {
	// TODO Auto-generated destructor stub
}
