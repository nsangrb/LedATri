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
#define sokieu 29
#define soluong_led 7
#define time_scale 10
void f_led(uint16_t led, UINT32 time);
void f_led_mo(uint16_t led, uint16_t led_mo, uint16_t led_dam, UINT32 phan5muoi, UINT32 *time);
void Sanglanluot(uint8_t solan, int mode, UINT32 *time);
void Sanglanluot_nguoc(uint8_t solan, int mode, UINT32 *time);
void Sangdan(uint8_t solan, int mode, UINT32 *time);
void Sangdan_nguoc(uint8_t solan, int mode, UINT32 *time);
void Sangdan2(uint8_t solan, int mode, UINT32 *time);
void Sangdan2_nguoc(uint8_t solan, int mode, UINT32 *time);
void Sole(uint8_t solan, int mode, UINT32 *time);
void Nhapnhay(uint8_t solan, int mode, UINT32 *time);
void Saobang(uint8_t solan, UINT32 *time);
void Saobang2(uint8_t solan, UINT32 *time);
void Random(uint8_t solan, UINT32 *time);
void Khoitao_Thutu();
int gettimer();
void Chon_Thutu();
void Timer3_Delay1us(UINT32 u32CNT);


volatile char cnt,kieu,cnt_timer = 0;
volatile char thutu[sokieu];
volatile UINT32 _time;
volatile UINT32 time_factor = 1;

void Timer3_Delay1us(UINT32 u32CNT)
{
	T3CON = 0x00;                           		//Timer3 Clock = Fsys/128
    set_TR3;                                		//Trigger Timer3
    while (u32CNT != 0)
    {
        RL3 = LOBYTE(TIMER_DIV4_VALUE_1us); //Find  define in "Function_define.h" "TIMER VALUE"
        RH3 = HIBYTE(TIMER_DIV4_VALUE_1us);
        while ((T3CON&SET_BIT4) != SET_BIT4);		//Check Timer3 Time-Out Flag
        clr_TF3;
        u32CNT --;
    }
    clr_TR3;              
}
void f_led(uint16_t led, UINT32 time)
{
	uint16_t led_tmp = 0;
	led = led<<(9-soluong_led);
	led_tmp = 0x08&(led<<3) | 0x04&(led<<1) | 0x02&(led>>1) | 0x01&(led>>3);
	P0 = 0x1f &(led >> 4);
	P1 = 0x0f & led_tmp;
	//set_GPIO1;			
	Timer0_Delay1ms(time);
	//clr_GPIO1;
}
void f_led_mo(uint16_t led, uint16_t led_mo, uint16_t led_dam, UINT32 phantram, UINT32 *time)
{
	UINT32 cnt_time=0;
	uint16_t led_tmp = 0,led_mo_tmp = 0,led_dam_tmp = 0;
	led_tmp = 0x08&(led<<3) | 0x04&(led<<1) | 0x02&(led>>1) | 0x01&(led>>3);
	led_mo_tmp = 0x08&(led_mo<<3) | 0x04&(led_mo<<1) | 0x02&(led_mo>>1) | 0x01&(led_mo>>3);
	led_dam_tmp = 0x08&(led_dam<<3) | 0x04&(led_dam<<1) | 0x02&(led_dam>>1) | 0x01&(led_dam>>3);
	for (cnt_time=0; cnt_time<((*time<4)?1:(int)(*time/4)); cnt_time++)
	{
		P0 = 0x1f &((led&(~led_dam)) >> 4);
		P1 = 0x0f & (led_tmp&(~led_dam_tmp));
		Timer3_Delay1us(phantram);
		P0 = 0x1f &((led&(~led_mo)|led_dam) >> 4);
		P1 = 0x0f & (led_tmp&(~led_mo_tmp)|led_dam_tmp);
		Timer3_Delay1us(100 - phantram);
	}
}
void Sanglanluot(uint8_t solan, int mode, UINT32 *time)
{
	int i = 0;
	for (i=0;i<solan;i++)
	{
		int j =0;
		uint16_t led = 0x0001;
		uint16_t led_tmp = led;
		if (mode)
		{
			led_tmp = ~led;
		}
		for (j = 1; j <= soluong_led + 1; j++)
		{
			f_led(led_tmp,(*time)*time_scale);
			led = 1 << j;
			if (mode)
			{
				led_tmp = ~led;
			}
			else
			{
				led_tmp = led;
			}
		}
	}
}

void Sanglanluot_nguoc(uint8_t solan, int mode, UINT32 *time)
{
	int i = 0; 
	for (i=0;i<solan;i++)
	{
		int j =0;
		uint16_t led = 1 << (soluong_led-1);
		uint16_t led_tmp = led;
		if (mode)
		{
			led_tmp = ~led;
		}
		for (j = 1; j <= soluong_led + 1; j++)
		{
			f_led(led_tmp,(*time)*time_scale);
			led = ((1 << (soluong_led-1)) >> j);
			if (mode)
			{
				led_tmp = ~led;
			}
			else
			{
				led_tmp = led;
			}
		}
	}
}

void Sangdan(uint8_t solan, int mode, UINT32 *time)
{
	int i = 0;
	for (i=0;i<solan;i++)
	{
		int j =0;
		uint16_t led = 0x0000; 
		uint16_t led_tmp = led;
		if (mode)
		{
			led_tmp = ~led;
		}
		for (j = 0; j < soluong_led + 1; j++)
		{
			f_led(led_tmp,(*time)*time_scale);
			led += (1 << j);
			if (mode)
			{
				led_tmp = ~led;
			}
			else
			{
				led_tmp = led;
			}
		}
	}
}

void Sangdan_nguoc(uint8_t solan, int mode, UINT32 *time)
{
	int i = 0;
	for (i=0;i<solan;i++)
	{
		int j =0;
		uint16_t led = 0x0000; 
		uint16_t led_tmp = led;
		if (mode)
		{
			led_tmp = ~led;
		}
		for (j = 0; j < soluong_led+1; j++)
		{
			f_led(led_tmp,(*time)*time_scale);
			led += ((1 << (soluong_led-1)) >> j);
			if (mode)
			{
				led_tmp = ~led;
			}
			else
			{
				led_tmp = led;
			}
		}
	}
}

void Sangdan2(uint8_t solan, int mode, UINT32 *time)
{
	int i = 0;
	for (i=0;i<solan;i++)
	{
		int j,k,n,tmp = 0;
		uint16_t led = 0x0000; 
		uint16_t led_tmp = led;
		if (mode)
		{
			led_tmp = ~led;
		}
		n = soluong_led;
		for (k = 0; k < soluong_led; k++)
		{
			for (j = 0; j < n; j++)
			{
				tmp += 0x0001 << n;
				f_led(led_tmp,(*time)*time_scale);
				led = (0x0001 << j) + tmp;
				if (mode)
				{
					led_tmp = ~led;
				}
				else
				{
					led_tmp = led;
				}
			}
			n--;
		}
		led = 0x01ff;
		if (mode)
		{
			led_tmp = ~led;
		}
		else
		{
			led_tmp = led;
		}
		f_led(led_tmp,(*time)*time_scale);
	}
}

void Sangdan2_nguoc(uint8_t solan, int mode, UINT32 *time)
{
	int i = 0;
	for (i=0;i<solan;i++)
	{
		int j,k,n,tmp = 0;
		uint16_t led = 0x0000; 
		uint16_t led_tmp = led;
		if (mode)
		{
			led_tmp = ~led;
		}
		n = soluong_led;
		for (k = 0; k < soluong_led; k++)
		{
			for (j = 0; j < n; j++)
			{
				tmp += (1 << (soluong_led-1)) >> n;
				f_led(led_tmp,(*time)*time_scale);
				led = ((1 << (soluong_led-1)) >> j) + tmp;
				if (mode)
				{
					led_tmp = ~led;
				}
				else
				{
					led_tmp = led;
				}
			}
			n--;
		}
		led = 0x01ff;
		if (mode)
		{
			led_tmp = ~led;
		}
		else
		{
			led_tmp = led;
		}
		f_led(led_tmp,(*time)*time_scale);
	}
}
void Sangdan3(uint8_t solan, UINT32 *time)
{
	int i = 0;
	for (i=0;i<solan;i++)
	{
		int j = 0;
		for (j = 0; j <= 100; j++)
		{
			f_led_mo(0x01ff,0x01ff,0x0000,j,time);
		}
	}
}
void Sangdan3_nguoc(uint8_t solan, UINT32 *time)
{
	int i = 0;
	for (i=0;i<solan;i++)
	{
		int j = 50;
		for (j = 100; j >= 0; j--)
		{
			f_led_mo(0x01ff,0x01ff,0x0000,j,time);
		}
	}
}
void Sole(uint8_t solan, int mode, UINT32 *time)
{
	int i = 0;
	for (i=0;i<solan;i++)
	{
		int j;
		uint16_t led = 0x0000; 
		uint16_t led_tmp = led;
		if (mode)
		{
			led_tmp = ~led;
		}
		for (j = 0; j < soluong_led-2; j++)
		{
			led = (0x0005) << j;
			if (mode)
			{
				led_tmp = ~led;
			}
			else
			{
				led_tmp = led;
			}
			f_led(led_tmp,(*time)*time_scale);
		}		
    for (j = 0; j < soluong_led-3; j++)
		{
			led = (0x0005<<(soluong_led-4)) >> j;
			if (mode)
			{
				led_tmp = ~led;
			}
			else
			{
				led_tmp = led;
			}
			f_led(led_tmp,(*time)*time_scale);
		}			
		led = 0;
		if (mode)
		{
			led_tmp = ~led;
		}
		else
		{
			led_tmp = led;
		}
		f_led(led_tmp,(*time)*time_scale);
	}
}
void Nhapnhay(uint8_t solan, int mode, UINT32 *time)
{
	int i = 0;
	for (i=0;i<solan;i++)
	{
		int j,k,tmp = 0;
		for (j = 0; j < soluong_led; j++)
		{
			for (k = 0; k < 4; k++)
			{
				if (mode)
				{
					f_led(~tmp,(*time)*time_scale);
					f_led(0,(*time)*time_scale);					
				}
				else
				{
					f_led(tmp,(*time)*time_scale);
					f_led(0x01ff,(*time)*time_scale);
				}				
			}
			tmp += 1 << j;
		}
	}
}
void Saobang(uint8_t solan, UINT32 *time)
{
	int i = 0;
	for (i=0;i<solan;i++)
	{
		int j = 0;
		uint16_t tmp;
		uint16_t led = 0x0000; 
		for (j = 0; j < soluong_led; j++)
		{
			int k = 0;
			tmp = 1 << j;
			led += tmp;
			for (k = 0; k<=100; k ++)
			{
				f_led_mo(led,tmp,0x0000,k,time);
			}
		}
		f_led(0x01ff,(*time)*time_scale);
	}
}
void Saobang2(uint8_t solan, UINT32 *time)
{
	int i = 0;
	for (i=0;i<solan;i++)
	{
		int j = 0;
		for (j = 2; j < soluong_led+4; j++)
		{
			f_led(0x00FC&(0xf000>>(16 - j)),(*time)*time_scale);
		}
		for (j = 1; j < soluong_led+1; j++)
		{
			f_led((0x00FC&(0xf000>>(j+4))),(*time)*time_scale);
		}
	}
}
void Saobang3(uint8_t solan, UINT32 *time)
{
	int i = 0;
	for (i=0;i<solan;i++)
	{
		int j = 0;
		for (j = 0; j < soluong_led+5; j++)
		{
			int k = 0;
			for (k = 0; k<=100; k ++)
			{
				f_led_mo((0xf000>>(16 - j)),(0x8000>>(16 - j)),(0x1000>>(16 - j)),k,time);
			}
		}
		for (j = soluong_led+4; j >= 0; j--)
		{
			int k = 0;
			for (k = 0; k<=100; k ++)
			{
				f_led_mo((0xf000>>(16 - j)),(0x8000>>(16 - j)),(0x1000>>(16 - j)),k,time);
			}
		}
	}
}
void Random(uint8_t solan, UINT32 *time)
{
	int i,r;
	srand(gettimer());
	for(i = 0; i < solan; ++i)
	{
        r = rand();
        f_led(r&0x01ff,(*time)*time_scale);
  }    
}



void Khoitao_Thutu()
{
	int i = 0;
	for (i = 0; i < sokieu; i ++)
	{
		thutu[i] = i;
	}
}
int gettimer()
{
	int timer;
	timer = TH1 << 8 + TL1;
	return timer;
}

void Chon_Thutu()
{
	int stack = sokieu;
	int tmp;
	while (stack > 0)
	{
		int a = 0;
		srand(gettimer());
		a = rand()%stack + sokieu - stack;;
		tmp = thutu[sokieu - stack];
		thutu[sokieu - stack] = thutu[a];
		thutu[a] = tmp;
		stack--;
	}
}

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
			time_factor = 40 - (int)((_time*40)>>12);
			CKDIV = 0x00;							
			Disable_ADC;
		}
		cnt_timer = ++cnt_timer%5;
}

void main (void) 
{
	Set_All_GPIO_Quasi_Mode;					// Define in Function_define.h
	P00_PushPull_Mode;
	P01_PushPull_Mode;
	P02_PushPull_Mode;
	P03_PushPull_Mode;
	P04_PushPull_Mode;
	P10_PushPull_Mode;
	P11_PushPull_Mode;
	P12_PushPull_Mode;
	P13_PushPull_Mode;

	TIMER1_MODE1_ENABLE;
	Enable_ADC_AIN0;
#if 0
	//InitialUART0_Timer3(115200);
//	set_CLOEN;  										// For clock out from P1.1
  
  while(1)
  {											// Tiny board GPIO1 LED define
		for (cnt = 0; cnt <= 50; cnt++)
		{
			f_led_mo(0x01ff,0x01ff,cnt,1);
		}
		for (cnt = 50; cnt >= 0; cnt--)
		{
			f_led_mo(0x01ff,0x01ff,cnt,1);
		}
  }
#endif

#if 1
	Khoitao_Thutu();
	clr_T1M;
  TH1 = 0;
  TL1 = 0;
	set_EA;       
	set_ET1;
	set_TR1;
	while(1)
  {
		Chon_Thutu();
		for (kieu = 0; kieu < sokieu; kieu++)
		{
			/*thutu[kieu]*/
			switch(thutu[kieu])
			{
				case 0:
						Sanglanluot(3,0,&time_factor);
						break;
				case 1:
						Sanglanluot_nguoc(3,0,&time_factor);
						break;
				case 2:
						for (cnt = 0; cnt <= 2; cnt++)
						{
							Sanglanluot(1,0,&time_factor);
							Sanglanluot_nguoc(1,0,&time_factor);
						}
						break;
				case 3:		
						Sangdan(3,0,&time_factor);
						break;
				case 4:
						Sangdan_nguoc(3,0,&time_factor);
						break;
				case 5:
						for (cnt = 0; cnt <= 2; cnt++)
						{
							Sangdan(1,0,&time_factor);
							Sangdan_nguoc(1,1,&time_factor);
						}
						break;
				case 6:
						for (cnt = 0; cnt <= 8; cnt++)
						{
							f_led(0x01ff,time_factor);
							f_led(0x0000,time_factor);
						}
						break;
				case 7:
						Sanglanluot(3,1,&time_factor);
						break;
				case 8:
						Sanglanluot_nguoc(3,1,&time_factor);
						break;
				case 9:
						for (cnt = 0; cnt <= 2; cnt++)
						{
							Sanglanluot(1,1,&time_factor);
							Sanglanluot_nguoc(1,1,&time_factor);
						}
						break;
				case 10:
						for (cnt = 0; cnt <= 2; cnt++)
						{
							Sangdan2_nguoc(1, 0,&time_factor);
							Sangdan2_nguoc(1, 1,&time_factor);
						}
						break;
				case 11:		
						Sangdan(3,1,&time_factor);
						break;
				case 12:
						Sangdan_nguoc(3,1,&time_factor);
						break;
				case 13:
						for (cnt = 0; cnt <= 2; cnt++)
						{
							Sangdan(1,1,&time_factor);
							Sangdan_nguoc(1,1,&time_factor);
						}
						break;
				case 14:
						for (cnt = 0; cnt <= 2; cnt++)
						{
							Sangdan(1,0,&time_factor);
							Sanglanluot_nguoc(1,1,&time_factor);
							Sangdan(1,1,&time_factor);
							Sanglanluot_nguoc(1,0,&time_factor);
						}
						break;
				case 15:		
						for (cnt = 0; cnt <= 2; cnt++)
						{
							Sangdan2(1, 0,&time_factor);
							Sangdan2_nguoc(1, 1,&time_factor);
							Sangdan2_nguoc(1, 0,&time_factor);
							Sangdan2(1, 1,&time_factor);
						}
						break;
				case 16:
						for (cnt = 0; cnt <= 2; cnt++)
						{
							Sangdan(1,1,&time_factor);
							Sole(1,0,&time_factor);
							Sangdan_nguoc(1,0,&time_factor);
						}
						break;
				case 17:
						Sole(5,1,&time_factor);
						break;
				case 18:
						for (cnt = 0; cnt <= 2; cnt++)
						{
							Sangdan(1,0,&time_factor);
							Sole(1,1,&time_factor);
							Sangdan_nguoc(1,1,&time_factor);
						}
						break;
				case 19:
						Sole(3,0,&time_factor);
						break;
				case 20:
						for (cnt = 0; cnt <= 2; cnt++)
						{
							Nhapnhay(1,0,&time_factor);
							Nhapnhay(1,1,&time_factor);
						}
						break;
				case 21:		
						Random(40,&time_factor);
						break;
				case 22:
						for (cnt = 0; cnt <= 8; cnt++)
						{
							f_led(0x0155,time_factor);
							f_led(0x00AA,time_factor);
						}
						break;
				case 23:
						for (cnt = 0; cnt <= 2; cnt++)
						{
							Sangdan3(1,&time_factor);
							Sangdan3_nguoc(1,&time_factor);
						}
						break;
				case 24:
						Saobang(3,&time_factor);
						break;
				case 25:
						Saobang2(3,&time_factor);
						break;
				case 26:
						Saobang3(3,&time_factor);
						break;
				case 27:
						for (cnt = 0; cnt <= 8; cnt++)
						{
							f_led(0x00CC,time_factor);
							f_led(0x01FF,time_factor);
						}
						break;
				case 28:
						for (cnt = 0; cnt <= 8; cnt++)
						{
							f_led(0x0038,time_factor);
							f_led(0x01FF,time_factor);
						}
						break;
				default:
						Khoitao_Thutu();
						break;
			}
	}
}
#endif
	

}



