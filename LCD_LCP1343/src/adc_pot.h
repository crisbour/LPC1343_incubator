/*
 * adc_pot.h
 *
 *  Created on: 9 Oct 2019
 *      Author: bourc
 */

#ifndef ADC_POT_H_
#define ADC_POT_H_

extern int AD0Result;

#define ADC_PWRUP  (~(1<<4)) //setting it to 0 will power it up
#define ADC_CLK_EN (1<<13) //Enable ADC clock
#define SEL_AD0    (1<<0) //Select Channel AD0
#define CLKDIV     15 // ADC clock-divider (ADC_CLOCK=PCLK/CLKDIV+1), yields 4.5Mhz ADC clock
#define ADC_BURST  (1<<16) //Enable burst(hardware) mode
#define ADCR_SETUP_BURST ((CLKDIV<<8) | ADC_BURST)


void initADC(void);
void ADC_IRQHandler(void);
void readPot(void);

#endif /* ADC_POT_H_ */
