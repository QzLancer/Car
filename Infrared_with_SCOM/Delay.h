#include <reg52.h>
#ifndef _DELAY_H_
#define _DELAY_H_

void Delay(int Time_ms)
{
		TMOD=0x01;	//TMOD�Ĵ������ã�������ʱ���ж�0
		TH0=(65536-1000)/256;	//װ�س�ֵ
		TL0=(65536-1000)%256;	//װ�س�ֵ
		EA=1;	//����ȫ���ж�
		ET0=1;	//������ʱ��0�ж�
		TR0=1;	//��ʱ��0ʹ��
	for(;Time_ms>0;Time_ms--)
	{	
		while(TF0==0);	//��ʱ��0δ���ʱ��������Delay����
	}
}

void Time0Interrupt(void) interrupt 1
{	
	TH0=(65536-1000)/256;	//װ�س�ֵ
	TL0=(65536-1000)%256;	//װ�س�ֵ
	TF0=0;	//�����־λ��λ
}

#endif