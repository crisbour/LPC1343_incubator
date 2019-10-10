/*
 * ds18b20.cpp
 *
 *  Created on: 4 Oct 2019
 *      Author: bourc
 */
#include "LPC13xx.h"
#include "ds18b20.h"
#include "Delay.h"


#define LED(val) ((val) ? (LPC_GPIO3->DATA |= (1<<0)) : (LPC_GPIO3->DATA&=~(1<<0)))

//Initialize sensor
DS18B20::DS18B20(uint8_t port,uint8_t pin){
	bitmask=1<<pin;
	switch(port){
	case 0:
		baseReg=(LPC_GPIO_TypeDef *)LPC_GPIO0_BASE;
		break;
	case 1:
		baseReg=(LPC_GPIO_TypeDef *)LPC_GPIO1_BASE;
		break;
	case 2:
		baseReg=(LPC_GPIO_TypeDef *)LPC_GPIO2_BASE;
		break;
	case 3:
		baseReg=(LPC_GPIO_TypeDef *)LPC_GPIO3_BASE;
		break;
	}
}


//Each communication through DS18b20 begin with a reset pulse. This is use for resetting Ds18b20 sensor.

uint8_t DS18B20::ow_reset(void)
{
    DQ_TRIS(1); // Tris = 0 (output)
	DQ(0); // set pin# to low (0)
	delayMicroseconds(480); // 1 wire require time delay
	DQ_TRIS(0); // Tris = 1 (input)
	delayMicroseconds(60); // 1 wire require time delay

	if (DQ_READ == 0) // if there is a presence pluse
	{
	delayMicroseconds(480);
	return 0; // return 0 ( 1-wire is presence)
	}
	else
	{
	delayMicroseconds(480);
	return 1; // return 1 ( 1-wire is NOT presence)
	}

} // 0=presence, 1 = no part

// This is use for reading bit from ds18b20

uint8_t DS18B20::read_bit(void)
{
	DQ_TRIS(1);
    DQ(0); // pull DQ low to start timeslot
	delayMicroseconds(1);
	DQ(1); // then return high
	DQ_TRIS(0);
	delayMicroseconds(15);
	bool val=DQ_READ;
	delayMicroseconds(50);
	return val; // return value of DQ line
}

uint8_t DS18B20::read_byte(void)
{
    char i,result = 0;
	DQ_TRIS(0); // TRIS is input(1)
		for(i = 0; i < 8; i++)
		result |= read_bit()<<i;
	return result;
}

//writes a bit to the one-wire bus, passed in bitval


void DS18B20::write_bit(bool bitval)
{
	DQ_TRIS(1);
    DQ(0); // pull DQ low to start timeslot
	delayMicroseconds(1);
	if(bitval){
		DQ(1); // return DQ high if write 1
		DQ_TRIS(0);
	}
	delayMicroseconds(60); // hold value for remainder of timeslot
	DQ(1);
    DQ_TRIS(0);
}// Delay provides 16us per loop, plus 24us. Therefore delay(5) = 104us

void DS18B20::write_byte(uint8_t val)
{
    char i;
    DQ_TRIS(0); // set pin# to input (1)

		for(i = 0; i < 8; i++)
			write_bit(val & (1<<i));

}

void DS18B20::init_sensProbe(void){
	ow_reset();
	write_byte(skip_rom);
	write_byte(write_scratchpad);
	write_byte(0);
	write_byte(0);
	write_byte(resolution_12bit);
	ow_reset();
}

void DS18B20::initReading(void){
	ow_reset();
	write_byte(skip_rom);
	write_byte(convert_temp);
	while(read_byte()==0xFF);
	delay(750);
}
float DS18B20::ReadTemp(void){
	uint8_t T_L,T_H;
	int16_t intTemp=0;
	float temp;

	initReading();
	ow_reset();
	write_byte(skip_rom);
	write_byte(read_scratchpad);
	T_L=read_byte();
	T_H=read_byte();


	if(T_H&0xF0)
		intTemp=1<<15;

	intTemp|=((T_H&0x07)<<8)|T_L;

	temp=((float)intTemp)/16;

	return temp;
}

char* StrTemp(float temp){
	char* errStr="Err";

	if(temp<0||temp>40) return errStr;

	static char str_temp[7]="00.00C";
	int value=(int)temp;
	str_temp[0]=(value/10)%10+'0';
	str_temp[1]=value%10+'0';
	value=(int)(temp*10);
	str_temp[2]='.';
	str_temp[3]=value%10+'0';
	value=(int)(temp*10);
	str_temp[4]=value%10+'0';

	return str_temp;
}
