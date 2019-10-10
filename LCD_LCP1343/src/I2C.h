/*
 * I2C.h
 *
 *  Created on: 2 Oct 2019
 *      Author: bourc
 */

#ifndef I2C_H_
#define I2C_H_

void I2C_init(void);
void beginTransmission(uint8_t address);
void sendData(uint8_t data);
void endTransmission();


#endif /* I2C_H_ */
