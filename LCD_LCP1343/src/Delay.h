/*
 * Delay.h
 *
 *  Created on: 2 Oct 2019
 *      Author: bourc
 */

#ifndef DELAY_H_
#define DELAY_H_

void delay(unsigned int milliseconds);
void delayMicroseconds(unsigned int milliseconds);
void initTimer0(void);

#define PRESCALE_US (72-1)
#define PRESCALE_MS (72000-1)


#endif /* DELAY_H_ */
