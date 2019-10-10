/*
 * I2C.cpp
 *
 *  Created on: 2 Oct 2019
 *      Author: bourc
 */


#include "LPC13xx.h"

#include "I2C.h"

void I2C_init(void){
	LPC_IOCON->PIO0_4			&=	~(0x303);	//clear "FUNC" and "I2CMODE
	LPC_IOCON->PIO0_4			|=	0x1; 		//PIO0_4 as SCL
	LPC_IOCON->PIO0_5			&=	~(0x303);	//clear "FUNC" and "I2CMODE
	LPC_IOCON->PIO0_5			|=	0x1; 		//PIO0_5 as SDA
	LPC_SYSCON->SYSAHBCLKCTRL 	|= 	(1<<5);		//Enable clock for I2C
	LPC_SYSCON->PRESETCTRL    	|= 	(0x2);    	//disable reset to I2C unit (sec 3.5.2)
	LPC_I2C->CONSET           	|= 	(1<<6);   	//put I2C unit in master transmit mode (sec 15.8.1 and 15.7.1)
	LPC_I2C->SCLH             	= 	350;     	//set clk dividers (sec 15.7.5) set arbitrarily long
	LPC_I2C->SCLL             	= 	350;     	//set clk dividers (sec 15.7.5) set arbitrarily long
}

void beginTransmission(uint8_t address){
	//I2C Start Transmission
	LPC_I2C->CONSET |= (1<<5);
	while((LPC_I2C->STAT & 0xF8) !=0x08);

	//Transmit device address
	LPC_I2C->DAT        = address<<1;
	LPC_I2C->CONCLR     = 0x28;		//Clear SIA and SI bit
}

void sendData(uint8_t data){
	while((LPC_I2C->STAT & 0xF8) != 0x18);          //wait for address byte to be sent
	LPC_I2C->DAT        = data;            //send data
	LPC_I2C->CONCLR     = (1<<3);			//clear SIC
}

void endTransmission(){
	 while((LPC_I2C->CONSET & 0x8) != 0x8); //set STOP bit
	 LPC_I2C->CONSET     = 0x10;            //set stop bit
	 LPC_I2C->CONCLR     = (1<<3);          //clear SIC
}
