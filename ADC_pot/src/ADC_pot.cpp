/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC13xx.h"
#endif
#include <stdio.h>

#include <cr_section_macros.h>

// TODO: insert other include files here
#include "Delay.h"

// TODO: insert other definitions and declarations here
#define VREF       3.3 //Reference Voltage at VREFP pin, given VREFN = 0V(GND)
#define ADC_CLK_EN (1<<13) //Enable ADC clock
#define SEL_AD0    (1<<0) //Select Channel AD0
#define CLKDIV      20	// ADC clock-divider (ADC_CLOCK=PCLK/CLKDIV+1), yields 4.5Mhz ADC clock
#define ADC_PWRUP  (~(1<<4)) //setting it to 0 will power it up
#define START_CNV  (1<<24) //001 for starting the conversion immediately
#define ADC_DONE   (1U<<31) //define it as unsigned value or compiler will throw warning
#define ADCR_SETUP_SCM (CLKDIV<<8)

#define LED(val) ((val) ? (LPC_GPIO3->DATA&=~(1<<0)):(LPC_GPIO3->DATA |= (1<<0)))
#define LED_TOG (LPC_GPIO3->DATA ^=1<<0)

unsigned int AD0Result=0;

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

int main(void) {

	LPC_GPIO3->DIR|=0xf;
	LED(0);
    // TODO: insert code here
	initTimer0();

	LPC_SYSCON->PDRUNCFG &= ADC_PWRUP; //Power-up ADC Block
	LPC_SYSCON->SYSAHBCLKCTRL |= ADC_CLK_EN; //Enable ADC clock
	LPC_ADC->CR =  ADCR_SETUP_SCM | SEL_AD0; //Setup ADC Block

	/* Now select AD0 function and set ADMODE=0 for PIO0_11(P0.11) */
	LPC_IOCON->R_PIO0_11 = 0x2;

	LPC_ADC->INTEN = SEL_AD0; //Enable AD0 Interrupt, set ADGINTEN to 0

	//NVIC_SetPriority(ADC_IRQn,0);
	NVIC_EnableIRQ(ADC_IRQn);
	__enable_irq();

	float voltage = 0;


    // Enter an infinite loop, just incrementing a counter
    while(1) {
		voltage = ((float)AD0Result * VREF) / 1024;

//		if(voltage>VREF/2.0)
//			LED(1);
//		else
//			LED(0);
		delay(100); //Slowing down Updates to 2 Updates per second
    }
    return 0 ;
}

