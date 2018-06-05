// 15通信 陈晓明 51单片机小车项目
#include <reg52.h>
#include<intrins.h>
#define uint unsigned int 
#define uchar unsigned char

#define fast1 63
#define slow1 42
#define fast2 63
#define slow2 42

uchar mode=0;

//定义两个电机的接口
sbit IA1=P1^5;	  
sbit IB1=P1^6;

sbit IA2=P2^6;
sbit IB2=P2^7;

sbit YOU=P2^1;
sbit ZUO=P2^0;

sbit Trig = P2^2;
sbit Echo = P2^3;

sbit hongwai_key=P3^4;
sbit lanya_key=P3^5;
sbit chaoshengbo_key=P3^6;

uchar pwn1=30;   	//右轮调速标志
uchar pwn2=30;		//左轮调速标志
uchar cycle;		//定义一个脉冲周期
uchar temp;			//存储蓝牙发送信息值

uchar go_back_flag;

unsigned char timer1;
unsigned char which=0;
unsigned char num=10,num1=20;

unsigned char timer3,t2; 
unsigned int time=0;
unsigned long S=0;

uint t_keyscan;
uint f,ff,fff;

void chaoshengbo();
void Time1Config();
void StartModule();
void Conut();
void hong();
void blueteeth();
void lanya();
void init();
void hongwai();	

void keyscan()
{
	static unsigned char state1=0,state2=0,state3=0;
	switch(state1)
	{
		case 0:
			if(hongwai_key==0)
				state1++;
			break;
		case 1:
			if(hongwai_key==0)
				state1++;
			else
				state1=0;
			break;
		case 2:
			if(hongwai_key==1)
			{
				state1=0;
				mode=0;
			}
			break;
	}	
	switch(state2)
	{
		case 0:
			if(lanya_key==0)
				state2++;
			break;
		case 1:
			if(lanya_key==0)
				state2++;
			else
				state2=0;
			break;
		case 2:
			if(lanya_key==1)
			{
				state2=0;
				mode=1;
			}
			break;
	}	
	switch(state3)
	{
		case 0:
			if(chaoshengbo_key==0)
				state3++;
			break;
		case 1:
			if(chaoshengbo_key==0)
				state3++;
			else
				state3=0;
			break;
		case 2:
			if(chaoshengbo_key==1)
			{
				state3=0;
				mode=2;
			}
			break;
	}	
}

//初始化串口、定时器
void init()
{
	TMOD=0x21;			  
	TH1=0xfd;
	TL1=0xfd;
	TR1=1;
	SCON=0X50;
	PCON=PCON&0x7f;
	ES=1;
	EA=1;	 
/*	TH0=(65536-500)/256;
	TH0=(65536-500)%256;
	ET0=1;
	TR0=1;	   */

	IA1=0;
	IB1=0;
	IA2=0;
	IB2=0;
}

void hongwai()
{
	hong();
	if(timer1>50)  
	{
		timer1=0;
	}
	if(timer1 < num)	
	{			  
		switch(which)
		{
			case 1:
				IA1=0;
				IB1=0;
				IA2=0;
				IB2=0; 
				break;
			case 2:
				IA1=1;
				IB1=0;	
				IA2=0;
				IB2=0;				
				break;
			case 3:
				IA1=0;
				IB1=0;
				IA2=1;
				IB2=0;
				break;
			case 4:
				IA1=1;
				IB1=0;	
				IA2=1;
				IB2=0;				
				break;
		}	
	}
	else
	{
		IA1=0;
		IB1=0;
		IA2=0;
		IB2=0; 
	}
}

void main()
{
	init();
	Time1Config();
	while(1)
	{
		if(t_keyscan>20)
		{
			t_keyscan=0;
			keyscan();
		}

		switch(mode)
		{
			case 0:
				lanya();
					break;
			case 1:
				hongwai();
					break;
			case 2:
				chaoshengbo();
					break;
		}	
	}
}

void timer2() interrupt 5
{
	if(mode==1||mode==2)
	{
		TH2=(65536-500)/256;
		TL2=(65536-500)%256;
	}
	else
	{
		TH2=(65536-250)/256;
		TL2=(65536-250)%256;
	}
	if(mode==0)
		cycle++;
	if(mode==1)
		timer1++; 
	if(mode==2)
	{	
		timer3++;
		t2++;
		if(fff)
		{
			ff++;
		}
	}
	t_keyscan++;
	
	TF2=0;
}

//驱动小车
void lanya()
{
	if(cycle>200)		  //同一个脉冲周期
	{
		cycle=0;
	}
	if(cycle<pwn1)		  //右轮
	{
		switch(temp)
		{
		case 0x38:	IA1=1;	IB1=0;	pwn1=fast1;	go_back_flag=1;		break;	  //正常速度前进
		case 0x34:	if(go_back_flag) {IA1=1;IB1=0;} else {IA1=0;IB1=1;}	pwn1=fast1;	break;	  //左转，左轮速度调小
		case 0x36:	if(go_back_flag) {IA1=1;IB1=0;} else {IA1=0;IB1=1;}	pwn1=slow1;	break;	  //右转，右轮速度调小
		case 0x32:	IA1=0;	IB1=1;	pwn1=fast1;	go_back_flag=0;		break;	  //正常速度后退
		case 0x37:	IA1=1;	IB1=0;	pwn1+=10; 	if(pwn1>95) pwn1=fast1;	break;	//加速
		case 0x39:	IA1=0;	IB1=0;	pwn1=fast1;	break;	  //刹车
		case 0x00:  IA1=0;	IB1=0;	pwn1=fast1;    break;
		}	
	}
	else
	{
		IA1=0;
		IB1=0;
	}
	if(cycle<pwn2)		   //左轮
	{
		switch(temp)
		{
		case 0x38:	IA2=1;	IB2=0;	pwn2=fast2;	go_back_flag=1;		break;	  //正常速度前进
		case 0x34:	if(go_back_flag) {IA2=1;IB2=0;} else {IA2=0;IB2=1;}	pwn2=slow2;	break;	  //左转，左轮速度调小
		case 0x36:	if(go_back_flag) {IA2=1;IB2=0;} else {IA2=0;IB2=1;}	pwn2=fast2;	break;	  //右转，右轮速度调小
		case 0x32:	IA2=0;	IB2=1;	pwn2=fast2;	go_back_flag=0;		break;	  //正常速度后退
		case 0x37:	IA2=1;	IB2=0;	pwn2+=10;	if(pwn2>95) pwn2=fast2;	break;	 //加速
		case 0x39:	IA2=0;	IB2=0;	pwn2=fast2;	break;	  //刹车
		case 0x00:  IA2=0;	IB2=0;	pwn2=fast2;    break;
		}
	}
	else
	{
		IA2=0;
		IB2=0;
	}
}

//串口通信
void uart()interrupt 4
{
	while(RI==0);
	temp=SBUF;
	RI=0;
	SBUF=temp;
	while(TI==0);
	TI=0;
}

void hong()
{
	if(YOU==0&&ZUO==0)
		which=4;
	if(YOU==1&&ZUO==0)
		which=2;
	if(YOU==0&&ZUO==1)
		which=3;  
	if(YOU==1&&ZUO==1)
		which=4;   	 
}  

void chaoshengbo()
{
	if(t2==50)
		{
			StartModule();
			while(!Echo);		//当RX为零时等待
			TR0=1;			    //开启计数
			while(Echo);			//当RX为1计数并等待
			TR0=0;				//关闭计数
			Conut();
			t2=0;
		}			

		if(timer3>50)  
		{
			timer3=0;
		}
		if(timer3 < num1)	
		{			  
			switch(which)
			{
				case 1:
					IA1=0;
					IB1=0;
					IA2=0;
					IB2=0;
					break;
				case 2:
					IA1=1;
					IB1=0;
					IA2=0;
					IB2=1;
					break;
				case 3:
					IA1=0;
					IB1=1;
					IA2=1;
					IB2=0;
					break;
				case 4:
					IA1=1;
					IB1=0;
					IA2=1;
					IB2=0;
					break;
				case 5:
					IA1=0;
					IB1=1;
					IA2=0;
					IB2=1;
					break;
			}	
		}
		else
		{
			IA1=0;
			IB1=0;
			IA2=0;
			IB2=0;
		} 	
}

void Time1Config()
{
	T2CON=0x00;
	TCLK=0;
	RCLK=0;
	RCAP2H=0x3c;
	RCAP2L=0xb0;


//	TMOD= 0x11;   
	TH2 = (65536-500)/256; 
	TL2 = (65536-500)%256;	
	ET2 = 1; 	 
	EA = 1;
	TR2 = 1;

	TH0=0;
	TL0=0;          
	ET0=1;             //允许T0中断		
}

void Conut()
{
	time=TH0*256+TL0;
	TH0=0;
	TL0=0;
	
	S=(time*1.7)/100;     //算出来是CM
	if(S<=17) //超出测量范围显示“-”
	{	 
		f++;
		if(f>3)
			f=0;
		which=5;  
		P2=0x00;
	}
	else
	{
		P2=0xff;
		if(f==0)
			which=4;
		else if(f==1)
			 {	
				fff=1;
				which=3;
				if(ff>50)
				{
					fff=0;
					ff=0;
					which=4;
					f=0;
				}
		   	 }
		else if(f==2)
			which=4;
		else if(f==3)
			 {
			 	 fff=1;
				 which=2;
				 if(ff>500)
				 {
				 	fff=0;
					ff=0;
					which=4;
					f=0;
				 }
			 }	
	}
}

void zd0() interrupt 1 		 //T0中断用来计数器溢出,超过测距范围
{
						 //中断溢出标志
}

void  StartModule() 		         //启动模块
{
	Trig=1;			                     //启动一次模块
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_();
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_();
	Trig=0;
}






























































			 







