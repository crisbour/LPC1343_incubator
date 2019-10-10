/*
 * adc_pot.cpp
 *
 *  Created on: 9 Oct 2019
 *      Author: bourc
 */

#include "LPC13xx.h"
#include "adc_pot.h"
#include "Delay.h"

#define LED_TOG (LPC_GPIO3->DATA ^=1<<0)

void initADC(){
	LPC_SYSCON->PDRUNCFG &= ADC_PWRUP; //Power-up ADC Block
	LPC_SYSCON->SYSAHBCLKCTRL |= ADC_CLK_EN; //Enable ADC clock
	LPC_ADC->CR =  ADCR_SETUP_BURST | SEL_AD0; //Setup ADC Block

	LPC_IOCON->R_PIO0_11 = 0x2;

	LPC_ADC->INTEN = SEL_AD0; //Enable AD0 Interrupt, set ADGINTEN to 0

}
void readPot(void){
	NVIC_EnableIRQ(ADC_IRQn); //Enable ADC IRQ
}
void ADC_IRQHandler(void)
{
	LED_TOG;
	unsigned long ADC_GDR_Read = LPC_ADC->GDR;
	int channel = (ADC_GDR_Read>>24) & 0x7; //Extract Channel Number
	int currentResult = (ADC_GDR_Read>>6) & 0x3FF; //Extract Conversion Result

	if(channel == 0)
	{
		AD0Result = LPC_ADC->DR0; //Dummy read to Clear Done bit
		AD0Result = currentResult;
		NVIC_DisableIRQ(ADC_IRQn);
	}
}
