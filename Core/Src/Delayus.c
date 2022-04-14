/*
 * Delayus.c
 *
 *  Created on: Apr 13, 2022
 *      Author: Rafal Mulinski
 */

#include "main.h"
#include "stm32f1xx_hal.h"
#include "tim.h"

#include "Delayus.h"

// Delay use timer 3
void Delay_us(uint16_t us)
{
	htim3.Instance->CNT = 0;
	while(htim3.Instance->CNT <= us);
}
