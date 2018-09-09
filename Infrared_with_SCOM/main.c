/*
�������ݷ��͵����ڣ�������9600
*/

#include <reg52.h>
//�궨�弰ȫ�ֱ���
#define TimeBase 0.125	//ʱ��Ϊ0.125ms
#define IR_Receive P3^2	//������ն�
int IR_TimeDiffGet[33]={0};	//����ɼ����½���ʱ���
unsigned char IR_Data[4]={0};	//�洢ת����ı���
int IR_Time=0;
bit IR_Finish=0;


//�Ӻ���
void InitTimer0();	//��ʼ����ʱ��0
void InitInt0();	//��ʼ���ⲿ�ж�0
void IR_Coding();
void DELAY_Us(unsigned int Us);	//764~928us
void DELAY_Ms(unsigned int Ms);	//1.007ms
void InitSCOM();
 

void main()
{	
	int i;
	bit SCOMFinish;	//���ڷ������
	InitTimer0();
	InitInt0();
	InitSCOM();
	while(1)
	{	
		if(IR_Finish==1)	//������־λδ��λ�����������
		{
			if(IR_Time>=100/TimeBase)	//�ж�ң���������Ƿ�������£����IR_Time����ʱ�����100ms����һ���ɿ�����λ
			{													//Ϊ�˷�ֹ�������IR_Time��ֵ����֤�����ɿ�����û�����ж�ʱ��ʼ����
				P1=0xff;
				IR_Time=100/TimeBase;
				IR_Finish=0;		//ȷ��û�а������º�ʹIR_Finish=0����֤����дP1
				SCOMFinish=0;	//����һ�ΰ�ťֻ����һ�����ݣ��ɿ���ť��λ
			}
			else
			{
				IR_Coding();
				P1=IR_Data[2];	//��������ж� ��ֵ
				for(i=0;i<=3&&SCOMFinish==0;i++)
				{
					SBUF=IR_Data[i];
					while(!TI);
					TI=0;
				}
				SCOMFinish=1;	//���������ݺ󣬴��ڷ��ͱ�־��λ
			}				
		}
		else
			P1=0xff;
	}
}

void InitTimer0()
{
	TMOD=0x22;	//ʹ�ö�ʱ��0����ʽ2,��ʱ��1���ڴ���
	TH0=0x8D;	//0.125msʱ����һ���ж�
	TL0=0x8D;	//�Զ�װ�س�ֵ
	ET0=1;	//������ʱ��0�ж�
	TR0=1;	//��ʱ��0��ʼ����
	EA=1;	//����ȫ���ж�
	PT0=1;	//��ʱ��0Ϊ�����ȼ��ж�
}

void InitInt0()
{
	IT0=1;	//�ⲿ�ж�0Ϊ�½��ش���
	EX0=1;	//�����ⲿ�ж�0
	EA=1;	//����ȫ���ж�
	PX0=0;	//�ⲿ�ж�0Ϊ�����ȼ��ж�
}

void InitSCOM()
{
	SM0=0;
	SM1=1;	//���ڹ����ڷ�ʽ1��8λԪUART����������TI��SMOD����
	PCON=0;
	TH1=0xFD;	//������9600
	TL1=0xFD;
	TR1=1;
}

void Timer0Int() interrupt 1 
{
	IR_Time++;	//ÿ������һ�Σ�IR_Time��1
	TF0=0;
}

void External0Int() interrupt 0 using 1
{
	static char i=0;
	static bit IR_StartFlag=0;//��ʼ������־λ
	IE0=0;	//�ⲿ�жϱ�־λ��λ
	if(IR_StartFlag==0)	//���������־λû����λ
	{
		if(IR_Time>=13/TimeBase&&IR_Time<=14/TimeBase)	//�����ȡ��ʱ������������Χ��
		{
			i=0;
			IR_StartFlag=1;	//��ʼ������־λ��λ
			IR_TimeDiffGet[i]=IR_Time;	//�����������ȴ���IR_TimeDiffGet[0]
			i++;	//����ָ��+1
		}
	}
	else if(IR_StartFlag==1&&i<=32)	//���������־λ�Ѿ���λ��������ָ��δ���
	{
		IR_TimeDiffGet[i]=IR_Time;	//��0����IR_TimeDiffGet[i]
		i++;	//����ָ��+1
		if(i<=32)					//���if ��else if�����Ƶ���� �ᵼ�°�����һ�ΰ���ʱ���ᱣ��һ��ʱ����һ�ΰ����Ľ��
			IR_Finish=0;
		else if(i>=33)
			IR_Finish=1;								
	}
	else	//���������־λ�Ѿ���λ��������ָ�����
	{
		IR_StartFlag=0;	//������־λ��λ
	}
	IR_Time=0;	//IR_Time��λ
}

void IR_Coding()
{
	int i,j;
	for(j=0;j<=3;j++)
	{
		for(i=1;i<=8;i++)
		{
			IR_Data[j]>>=1;	//IR_Data[j]����һλ
			if(IR_TimeDiffGet[i+8*j]>=1.5/TimeBase)	//���IR_TimeDiffGet�ﴢ���ֵ����1.5ms������ֵΪ1
				IR_Data[j]|=0x80;	//�����λ��1
		}
	}
	return;
}

void DELAY_Us(unsigned int Us)
{
	unsigned int x;
	for(x = 0; x <= (Us/200-1); x++);
}
void DELAY_Ms(unsigned int Ms)
{
	unsigned int x,y;
	for(x = 0; x <= (Ms-1); x++)
	{
		for(y = 0; y <= 120; y++);
	}
}
