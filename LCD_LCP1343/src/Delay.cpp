/*
 * Delay.cpp
 *
 *  Created on: 2 Oct 2019
 *      Author: bourc
 */

#include "LPC13xx.h"
#include "Delay.h"

void initTimer0(void)
{
	/*Assuming CCLK (System Clock) = 72Mhz (set by startup code)*/
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<9); //Enable 32Bit Timer0 Clock
	LPC_TMR32B0->CTCR = 0x0;
	LPC_TMR32B0->PR = PRESCALE_MS; //Increment LPC_TMR32B0->TC at every 71999+1 clock cycles
	//72000 clock cycles @72Mhz = 1 mS
	LPC_TMR32B0->TCR = 0x02; //Reset Timer
}

void delayMicroseconds(unsigned int microseconds) //Using Timer0
{
	LPC_TMR32B0->PR = PRESCALE_US;
	LPC_TMR32B0->TCR = 0x02; //Reset Timer
	LPC_TMR32B0->TCR = 0x01; //Enable timer

	while(LPC_TMR32B0->TC < microseconds); //wait until timer counter reaches the desired delay

	LPC_TMR32B0->TCR = 0x00; //Disable timer
}

void delay(unsigned int milliseconds) //Using Timer0
{
	LPC_TMR32B0->PR = PRESCALE_MS;
	LPC_TMR32B0->TCR = 0x02; //Reset Timer
	LPC_TMR32B0->TCR = 0x01; //Enable timer


	while(LPC_TMR32B0->TC < milliseconds); //wait until timer counter reaches the desired delay

	LPC_TMR32B0->TCR = 0x00; //Disable timer
}


