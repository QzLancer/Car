#include <reg52.h>
#ifndef _DELAY_H_
#define _DELAY_H_

void Delay(int Time_ms)
{
		TMOD=0x01;	//TMOD寄存器配置：开启定时器中断0
		TH0=(65536-1000)/256;	//装载初值
		TL0=(65536-1000)%256;	//装载初值
		EA=1;	//开启全局中断
		ET0=1;	//开启定时器0中断
		TR0=1;	//定时器0使能
	for(;Time_ms>0;Time_ms--)
	{	
		while(TF0==0);	//定时器0未溢出时，不跳出Delay程序
	}
}

void Time0Interrupt(void) interrupt 1
{	
	TH0=(65536-1000)/256;	//装载初值
	TL0=(65536-1000)%256;	//装载初值
	TF0=0;	//溢出标志位复位
}

#endif