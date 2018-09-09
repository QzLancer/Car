/*
红外数据发送到串口，波特率9600
*/

#include <reg52.h>
//宏定义及全局变量
#define TimeBase 0.125	//时基为0.125ms
#define IR_Receive P3^2	//红外接收端
int IR_TimeDiffGet[33]={0};	//存入采集的下降沿时间差
unsigned char IR_Data[4]={0};	//存储转换后的编码
int IR_Time=0;
bit IR_Finish=0;


//子函数
void InitTimer0();	//初始化定时器0
void InitInt0();	//初始化外部中断0
void IR_Coding();
void DELAY_Us(unsigned int Us);	//764~928us
void DELAY_Ms(unsigned int Ms);	//1.007ms
void InitSCOM();
 

void main()
{	
	int i;
	bit SCOMFinish;	//串口发送完毕
	InitTimer0();
	InitInt0();
	InitSCOM();
	while(1)
	{	
		if(IR_Finish==1)	//存数标志位未置位，即存数完成
		{
			if(IR_Time>=100/TimeBase)	//判断遥控器按键是否持续按下：如果IR_Time积累时间大于100ms，则一定松开，复位
			{													//为了防止溢出，给IR_Time赋值，保证按键松开，即没产生中断时灯始终灭
				P1=0xff;
				IR_Time=100/TimeBase;
				IR_Finish=0;		//确认没有按键按下后，使IR_Finish=0，保证不改写P1
				SCOMFinish=0;	//按下一次按钮只发送一组数据，松开按钮复位
			}
			else
			{
				IR_Coding();
				P1=IR_Data[2];	//如果产生中断 则赋值
				for(i=0;i<=3&&SCOMFinish==0;i++)
				{
					SBUF=IR_Data[i];
					while(!TI);
					TI=0;
				}
				SCOMFinish=1;	//发送完数据后，串口发送标志置位
			}				
		}
		else
			P1=0xff;
	}
}

void InitTimer0()
{
	TMOD=0x22;	//使用定时器0，方式2,定时器1用于串口
	TH0=0x8D;	//0.125ms时触发一次中断
	TL0=0x8D;	//自动装载初值
	ET0=1;	//开启定时器0中断
	TR0=1;	//定时器0开始运行
	EA=1;	//开启全局中断
	PT0=1;	//定时器0为高优先级中断
}

void InitInt0()
{
	IT0=1;	//外部中断0为下降沿触发
	EX0=1;	//开启外部中断0
	EA=1;	//开启全局中断
	PX0=0;	//外部中断0为低优先级中断
}

void InitSCOM()
{
	SM0=0;
	SM1=1;	//串口工作在方式1，8位元UART，波特率由TI和SMOD决定
	PCON=0;
	TH1=0xFD;	//波特率9600
	TL1=0xFD;
	TR1=1;
}

void Timer0Int() interrupt 1 
{
	IR_Time++;	//每计数满一次，IR_Time加1
	TF0=0;
}

void External0Int() interrupt 0 using 1
{
	static char i=0;
	static bit IR_StartFlag=0;//开始存数标志位
	IE0=0;	//外部中断标志位复位
	if(IR_StartFlag==0)	//如果存数标志位没有置位
	{
		if(IR_Time>=13/TimeBase&&IR_Time<=14/TimeBase)	//如果读取的时间在引导区域范围内
		{
			i=0;
			IR_StartFlag=1;	//开始存数标志位置位
			IR_TimeDiffGet[i]=IR_Time;	//将引导区长度存入IR_TimeDiffGet[0]
			i++;	//数组指针+1
		}
	}
	else if(IR_StartFlag==1&&i<=32)	//如果存数标志位已经置位，且数组指针未溢出
	{
		IR_TimeDiffGet[i]=IR_Time;	//将0存入IR_TimeDiffGet[i]
		i++;	//数组指针+1
		if(i<=32)					//如果if 和else if部分移到最后 会导致按下下一次按键时，会保持一段时间上一次按键的结果
			IR_Finish=0;
		else if(i>=33)
			IR_Finish=1;								
	}
	else	//如果存数标志位已经置位，且数组指针溢出
	{
		IR_StartFlag=0;	//存数标志位复位
	}
	IR_Time=0;	//IR_Time复位
}

void IR_Coding()
{
	int i,j;
	for(j=0;j<=3;j++)
	{
		for(i=1;i<=8;i++)
		{
			IR_Data[j]>>=1;	//IR_Data[j]右移一位
			if(IR_TimeDiffGet[i+8*j]>=1.5/TimeBase)	//如果IR_TimeDiffGet里储存的值大于1.5ms，即数值为1
				IR_Data[j]|=0x80;	//给最高位赋1
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
