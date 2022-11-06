/*
 * delay.h
 *
 *  Created on: 4 Nov 2022
 *      Author: Manish
 */

#ifndef SRC_DELAY_H_
#define SRC_DELAY_H_

#include <stdint.h>

void delay_systick(const int ticks);

#ifdef __cplusplus
extern "C" {
#endif
uint32_t millis();
#ifdef __cplusplus
}
#endif


#endif /* SRC_DELAY_H_ */
