#ifndef _MOTOR_CTRL_H_
#define _MOTOR_CTRL_H_

#include <reg52.h>
//123
sbit Motor1_0=P1^3;
sbit Motor1_1=P1^2;
sbit Motor1_EN=P1^4;
sbit Motor2_0=P1^6;
sbit Motor2_1=P1^7;
sbit Motor2_EN=P1^5;

void CarFwd()	//С�������˶�
{
	Motor1_EN=1;
	Motor1_0=1;
	Motor1_1=0;
	Motor2_EN=1;
	Motor2_0=1;
	Motor2_1=0;
}


void CarRev()	//С�������˶�
{
	Motor1_EN=1;
	Motor1_0=0;
	Motor1_1=1;
	Motor2_EN=1;
	Motor2_0=0;
	Motor2_1=1;
}

void CarStop()	//С��ֹͣ
{
	Motor1_EN=0;
	Motor1_0=0;
	Motor1_1=0;
	Motor2_EN=0;
	Motor2_0=0;
	Motor2_1=0;
}

void CarLeft()	//С����ת
{
	Motor1_EN=1;
	Motor1_0=0;
	Motor1_1=0;
	Motor2_EN=1;
	Motor2_0=1;
	Motor2_1=0;
}

void CarRight()	//С����ת
{
	Motor1_EN=1;
	Motor1_0=1;
	Motor1_1=0;
	Motor2_EN=1;
	Motor2_0=0;
	Motor2_1=0;
}

#endif