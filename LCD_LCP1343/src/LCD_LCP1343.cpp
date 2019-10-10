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

#include <stdlib.h>
#include <cr_section_macros.h>


// TODO: insert other include files here
#include "Delay.h"
#include "I2C.h"
#include "LiquidCrystal_I2C.h"
#include "ds18b20.h"
#include "adc_pot.h"

// TODO: insert other definitions and declarations here
LiquidCrystal_I2C lcd(0x27, 16, 2);
DS18B20 WT(2,4);
DS18B20 IT(2,7);

//#define abs(val) (((val)>=0) ? (val) : -(val))

#define LED(val) ((val) ? (LPC_GPIO3->DATA&=~(1<<0)):(LPC_GPIO3->DATA |= (1<<0)))
#define LED_TOG (LPC_GPIO3->DATA ^=1<<0)
#define HEATER(val) ((val) ? (LPC_GPIO0->DATA&=~(1<<8)):(LPC_GPIO0->DATA |= (1<<8)))
#define FAN(val) ((val) ? (LPC_GPIO1->DATA |= (1<<11)) : (LPC_GPIO1->DATA&=~(1<<11)))
#define BUT (((LPC_GPIO1->DATA)>>8)&0x1)

#define K_BALANCE 0.18
#define K_LOSS 2.5
#define T_AMBIENT 18
#define TEMP_TOL 0.5

#define POT_RANGE 270

void initTempSensors(void);
void verifyTemp(float* IT_val,float* WT_val);
void writeTempLCD(float,float);
void HeaterControl(float,float,float);
void FanControl(float,float);
void lightLCD(void);
void resetBoard(void);

int AD0Result = 0;

int main(void) {
	float targetTemp=27.00;
	float WT_val;
	float IT_val;
	float degree=0;
    // TODO: insert code here
	initTimer0();

	LPC_GPIO3->DIR |=  (1<<0)|(1<<1);		//Output for leds
	LPC_GPIO0->DIR |=  (1<<8);		//Output for heater relay
	LPC_GPIO1->DIR |=  (1<<11);		//Output for fan
	LPC_GPIO1->DIR &= ~(1<<8);		//Input for button
	FAN(0);							//Stop fan
	HEATER(0);						//Stop heater
	LED(0);
	LPC_GPIO3->DATA &=~(1<<1);

	//Initialize LCD
	lcd.init();
	lcd.backlight();

	//Initliaze ADC
	//initADC();

	//Initialize sensors
	initTempSensors();


	//Set LCD format
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.printstr("TARGET|");
    lcd.setCursor(0,1);
    lcd.printstr(StrTemp(targetTemp));
    lcd.printChar('|');

    while(1) {
//    	readPot();
//    	degree=((float)AD0Result*POT_RANGE)/1024;
    	WT_val=WT.ReadTemp();
    	IT_val=IT.ReadTemp();
    	verifyTemp(&IT_val, &WT_val);
    	writeTempLCD(IT_val,WT_val);
    	HeaterControl(targetTemp,IT_val,WT_val);
    	FanControl(targetTemp,IT_val);
    	lightLCD();
    	resetBoard();
    }
    return 0 ;
}

void initTempSensors(){

    // Enter an infinite loop, just incrementing a counter
    while(WT.ow_reset()){
    	lcd.setCursor(0,0);
    	lcd.printstr("Please Connect:");
    	lcd.setCursor(0,1);
    	lcd.printstr("TempSens Probe 1");
    }
    while(IT.ow_reset()){
		lcd.setCursor(0,0);
		lcd.printstr("Please Connect:");
		lcd.setCursor(0,1);
		lcd.printstr("TempSens Probe 2");
	}


    WT.init_sensProbe();
    IT.init_sensProbe();
    delay(1000);

    lcd.clear();
    lcd.setCursor(0,1);
    lcd.printstr("Init Successful");
    delay(1000);
}
void verifyTemp(float *IT_val,float *WT_val){
	static float it=0.00,wt=0.00;
	if(it<0.5&&wt<0.5){
		it=*IT_val;
		wt=*IT_val;
		return;
	}
	if(*IT_val>10.0 && *IT_val<40)
		it=*IT_val;
	else{
		*IT_val=it;
		IT.init_sensProbe();
	}

	if(*WT_val>10.0 && *WT_val<40)
		wt=*WT_val;
	else{
		*WT_val=wt;
		WT.init_sensProbe();
	}
}
void writeTempLCD(float IT_val,float WT_val){
	lcd.setCursor(7,1);
	lcd.printstr("WT:");
	lcd.printstr(StrTemp(WT_val));
	lcd.setCursor(7,0);
	lcd.printstr("IT:");
	lcd.printstr(StrTemp(IT_val));
}

void HeaterControl(float target,float IT_val,float WT_val){
	static uint8_t heater_val=1;
	float target_WT=K_BALANCE*(target-T_AMBIENT)+target;
	float del_WT_loss=K_LOSS*(target-IT_val);
	//Control when Incubator temperature is less than the target temp
	if(WT_val+TEMP_TOL/4.0<target_WT+del_WT_loss)
		heater_val=1;
	if(WT_val-TEMP_TOL/4.0>target_WT+del_WT_loss)
		heater_val=0;

	//Control when Incubator temp meets target temp
//	if(target<IT_val && WT_val<(target_WT-TEMP_TOL))
//		heater_val=1;
//	if(target<IT_val && WT_val>(target_WT+TEMP_TOL))
//		heater_val=0;

	HEATER(heater_val);
}
void lightLCD(){
	static int cnt=0;
	cnt++;
	if(BUT){
		lcd.backlight();
		cnt=0;
	}
	if(cnt>20){
		lcd.noBacklight();
		cnt=0;
	}
}
void FanControl(float target,float IT_val){
	static uint8_t fan_val=0;
	static int cnt=20000;

	cnt++;		//Each count is 1.5s worth

	if(IT_val<target-1.5*TEMP_TOL){
		if(fan_val)
			cnt=0;
		fan_val=0;
	}
	if(IT_val>target-TEMP_TOL && cnt>10000 && fan_val==0){
		fan_val=1;
		cnt=0;
	}
	if(cnt>100 && fan_val==1){
		fan_val=0;
		cnt=0;
	}

	FAN(fan_val);

}

void resetBoard(){
	static unsigned int cnt=0;
	cnt++;
	if(cnt>1000)
		NVIC_SystemReset();
}
