/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2017 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : Jan/21/2017
//***********************************************************************************************************

//***********************************************************************************************************
//  File Function: N76E003 GPIO demo code
//***********************************************************************************************************
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"
#include <stdio.h>
#include <stdlib.h>
//*****************  The Following is in define in Fucntion_define.h  ***************************
//****** Always include Function_define.h call the define you want, detail see main(void) *******
//***********************************************************************************************

#define time_scale 100
void Timer0_Delay1ms_ptr(UINT32 (*time));

volatile char cnt_timer = 0;
volatile UINT32 _time;
volatile UINT32 time_factor = 1;

void Timer1_ISR (void) interrupt 3  //interrupt address is 0x001B
{
    TH1 = 0;
    TL1 = 0;   
		if (cnt_timer == 4)
		{
			Enable_ADC_AIN0;
			CKDIV = 0x02;					
			clr_ADCF;
			set_ADCS;									// ADC start trig signal
			while(ADCF == 0);
			_time = 0x0fff & ((ADCRH << 4) | ADCRL);
			time_factor = 20 - (int)((_time*20)>>12);
			CKDIV = 0x00;							
			Disable_ADC;
		}
		cnt_timer = ++cnt_timer%5;
}
void Timer0_Delay1ms_ptr(UINT32 (*time))
{
	Timer0_Delay1ms((*time)*time_scale);
}
void main (void) 
{
	Set_All_GPIO_Quasi_Mode;					// Define in Function_define.h
	TIMER1_MODE1_ENABLE;
	Enable_ADC_AIN0;

	clr_T1M;
  TH1 = 0;
  TL1 = 0;
	set_EA;       
	set_ET1;
	set_TR1;
	while(1)
  {
		P0 = 0x04;
		Timer0_Delay1ms_ptr(&time_factor);
		P0 = 0x08;
		Timer0_Delay1ms_ptr(&time_factor);
		P0 = 0x10;
		Timer0_Delay1ms_ptr(&time_factor);
		P0 = 0x08;
		Timer0_Delay1ms_ptr(&time_factor);
	}

}



