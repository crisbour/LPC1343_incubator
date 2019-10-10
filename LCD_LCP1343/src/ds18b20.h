/*
 * ds18b20.h
 *
 *  Created on: 4 Oct 2019
 *      Author: bourc
 */

#ifndef DS18B20_H_
#define DS18B20_H_

//Macros for DQ manipulation
//#define DQ_READ 		(((LPC_GPIO2->DATA)>>4)&0x1)
//#define DQ(val) 		((val)?(LPC_GPIO2->DATA |= (1<<4)):(LPC_GPIO2->DATA&=~(1<<4)))
//#define DQ_TRIS(val) 	((val)?(LPC_GPIO2->DIR |= (1<<4)):(LPC_GPIO2->DIR&=~(1<<4)))

//Configuration Codes
#define skip_rom 0xCC
#define convert_temp 0x44
#define write_scratchpad 0x4E
#define resolution_12bit 0x7F
#define read_scratchpad 0xBE

char* StrTemp(float);


class DS18B20{
private:
	uint16_t bitmask;
	LPC_GPIO_TypeDef *baseReg;
	#define DQ_READ 		((baseReg->DATA)&bitmask)
	#define DQ(val) 		((val)?(baseReg->DATA |= bitmask):(baseReg->DATA&=~bitmask))
	#define DQ_TRIS(val) 	((val)?(baseReg->DIR |= bitmask):(baseReg->DIR&=~bitmask))
public:
	DS18B20(uint8_t port,uint8_t pin);

	//Functions
	uint8_t ow_reset(void);
	uint8_t read_bit(void);
	uint8_t read_byte(void);
	void write_bit(bool bitval);
	void write_byte(uint8_t val);
	void init_sensProbe(void);
	void initReading(void);
	float ReadTemp(void);

};


#endif /* DS18B20_H_ */
