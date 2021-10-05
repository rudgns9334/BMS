/*
 * Balancing_Init.c
 *
 *  Created on: 2021. 9. 14.
 *      Author: 82109
 */

#include "Defines.h"
#include "SIUL.h"


void SIUL_EIRQ20_Init(void){
	SIUL_EnableExtIntFallingEdge(SIUL_EIRQ20);
	INTC_0.PSR[245].R = 0x800A;
}

void GPIO_Init(void){
	SIUL_DigitalOutput(SIUL_PD4, SIUL_GPIO); // reserved
	SIUL_DigitalOutput(SIUL_PD5, SIUL_GPIO); // indicator 1
	SIUL_DigitalOutput(SIUL_PD6, SIUL_GPIO); // indicator 2
	SIUL_DigitalOutput(SIUL_PD7, SIUL_GPIO); // indicator 3
	SIUL_DigitalOutput(SIUL_PD8, SIUL_GPIO); // indicator 4
	SIUL_DigitalOutput(SIUL_PD9, SIUL_GPIO); // relay 1
	SIUL_DigitalOutput(SIUL_PD10, SIUL_GPIO); // relay 2
	SIUL_DigitalOutput(SIUL_PD11, SIUL_GPIO); // relay 3
	//SIUL_DigitalOutput(SIUL_PB2, SIUL_GPIO);
	SIUL_DigitalInput(SIUL_PB15, SIUL_IN_PB15_IO, SIUL_PULL_UP | SIUL_SLEW_ENB, SIUL_ALT1); // trigger to restart after charging stops.
// 20번 인터럽트
}
// SIUL_SetPad 이걸로 신호 줄 수 있다!!!!!
