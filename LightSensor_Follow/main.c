/*
���⴫�����Զ����ٳ���
*/
//�ڶ����޸�
#include <reg52.h>
#include "MotorCtrl.h"
sbit IR_Left=P3^4;
sbit IR_Right=P3^5;

void main()
{
	while(1)
	{
		if(IR_Left==1&&IR_Right==1)
			CarFwd();
		else if(IR_Left==0&&IR_Right==0)
			CarRev();
		else if(IR_Left==1&&IR_Right==0)
			CarLeft();
		else if(IR_Left==0&&IR_Right==1)
			CarRight();
	}
}

