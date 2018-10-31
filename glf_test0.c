#include <STC12C5A60S2.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char


sbit LE0 = P2^0;
sbit LE1 = P2^1;
sbit LE2 = P2^2;
sbit LE3 = P2^3;
sbit LE4 = P2^4;
sbit LE5 = P2^5;
sbit LE6 = P2^6;
sbit LE7 = P2^7;
sbit LE = P3^7;

uint flag = 0;
int temp = 0x00;
int count = 0;
int count1 = 0;
int count2 = 0;
int count3 = 0;
int count4 = 0;



void init_led();
void init_timer0();
void task_led0();
void task_led1();

void init_timer0()
{
//	TMOD = 0x01;
	TL0 = (65536-1000)%256;
	TH0 = (65536-1000)/256;
	TR0 = 1;
	ET0 = 1;
	EA = 1;
	count = 0;
}

void timer0() interrupt 1
{
	TL0 = (65536-1000)%256;
	TH0 = (65536-1000)/256;
	switch(flag)
	{
		case 0x00: count = 0; count1 = 0; count2 = 0; count3 = 0;init_led();break;
		case 0x01:  count ++;
					if(count == 300)
					{
						count = 0;
						task_led1();
			
					} break;
		case 0x02:	count1 ++;
					if(count1 == 150)
					{
						count1 = 0;
						task_led0();
					}break;
		case 0x03:	count2 ++;
					if(count2 == 50)
					{
						count2 = 0;
						task_led0();
					}break;
		case 0x04: 	count3 ++;
					if(count3 == 45)
					{
						count3 = 0;
						task_led0();
					}break;
		case 0x05:	count2 ++;
					if(count2 == 40)
					{
						count2 = 0;
						task_led1();
					}break;
	}
	
/*	if(count == 1000)
	{
		count = 0;
		LE = 1;
		P1 = temp;
		LE = 0;
		temp = ~temp;
	}	*/
}

void uart_init()
{
	TMOD = 0x21;
	TH1 = 0xfd;
	TL1 = 0xfd;
	TR1 = 1;
	PCON = 0x00;
	SCON = 0x50;
	ES = 1;
	EA = 1;
}

void uart_isr() interrupt 4
{
	if(RI)
	{
		RI = 0;
	    flag = SBUF;
	}
}

void init_led()
{
	LE0 = 1;
	P0 = 0xff;
	LE0 = 0;

	LE1 = 1;
	P0 = 0xff;
	LE1 = 0;

	LE2 = 1;
	P0 = 0xff;
	LE2 = 0;

	LE3 = 1;
	P0 = 0xff;
	LE3 = 0;

	LE4 = 1;
	P0 = 0xff;
	LE4 = 0;

	LE5 = 1;
	P0 = 0xff;
	LE5 = 0;

	LE6 = 1;
	P0 = 0xff;
	LE6 = 0;

	LE7 = 1;
	P0 = 0xff;
	LE7 = 0;

	LE = 1;
	P1 = 0x00;
	LE = 0;
}

void delay(uint z)
{
	uint x, y;
	for(y=110; y>0; y--)
		for(x=z; x>0; x--);
}

void task_led0()
{
	static uint i = 0;
	static uint up = 0x80;
	static uint down = 0x01;
	if(i == 0)
		init_led();

	if(i < 16)
	{LE = 1;
	P1 = 0xff;
	LE = 0;
	}
	
	switch(i)
	{
		case 0: LE7 = 1; P0 = 0xff; LE7 = 0; LE0 = 1; P0 = 0x00; LE0 = 0; break;
		case 1: LE0 = 1; P0 = 0xff; LE0 = 0; LE1 = 1; P0 = 0x00; LE1 = 0; break;
		case 2: LE1 = 1; P0 = 0xff; LE1 = 0; LE2 = 1; P0 = 0x00; LE2 = 0; break;
		case 3: LE2 = 1; P0 = 0xff; LE2 = 0; LE3 = 1; P0 = 0x00; LE3 = 0; break;
		case 4: LE3 = 1; P0 = 0xff; LE3 = 0; LE4 = 1; P0 = 0x00; LE4 = 0; break;
		case 5: LE4 = 1; P0 = 0xff; LE4 = 0; LE5 = 1; P0 = 0x00; LE5 = 0; break;
		case 6: LE5 = 1; P0 = 0xff; LE5 = 0; LE6 = 1; P0 = 0x00; LE6 = 0; break;
		case 7: LE6 = 1; P0 = 0xff; LE6 = 0; LE7 = 1; P0 = 0x00; LE7 = 0; break;
		case 8: P0 = 0xfe; 	LE0 = 1;
				LE0 = 0;
				LE1 = 1;
				LE1 = 0;
				LE2 = 1;
				LE2 = 0;
				LE3 = 1;
				LE3 = 0;
				LE4 = 1;
				LE4 = 0;
				LE5 = 1;
				LE5 = 0;
				LE6 = 1;
				LE6 = 0;
				LE7 = 1;
				LE7 = 0;
				P0 = _crol_(P0, 1);break;
		case 9: case 10: case 11: case 12: case 13: case 14: case 15:
		case 16: case 17: case 18: case 19: case 20: case 21: case 22:
		case 23:
				LE0 = 1;
				LE0 = 0;
				LE1 = 1;
				LE1 = 0;
				LE2 = 1;
				LE2 = 0;
				LE3 = 1;
				LE3 = 0;
				LE4 = 1;
				LE4 = 0;
				LE5 = 1;
				LE5 = 0;
				LE6 = 1;
				LE6 = 0;
				LE7 = 1;
				LE7 = 0;
				P0 = _crol_(P0, 1);
		break;
		case 24:	LE = 1;
					P1 = 0x00;
					LE = 0;
				
					LE0 = 1;
					P0 = 0x00;
					LE0 = 0;
				
					LE1 = 1;
					P0 = 0x00;
					LE1 = 0;
				
					LE2 = 1;
					P0 = 0x00;
					LE2 = 0;
				
					LE3 = 1;
					P0 = 0x00;
					LE3 = 0;
				
					LE4 = 1;
					P0 = 0x00;
					LE4 = 0;
				
					LE5 = 1;
					P0 = 0x00;
					LE5 = 0;
				
					LE6 = 1;
					P0 = 0x00;
					LE6 = 0;
				
					LE7 = 1;
					P0 = 0x00;
					LE7 = 0;

					LE = 1;
		P1 = up | down;
		LE = 0;
	//	delay(1500);
		if(i==2 || i==9)
		{
			up = _cror_(up, 2);
			down = _crol_(down, 2);	
		}
		else
		{
			up = _cror_(up, 1);
			down = _crol_(down, 1);
		}
		break;
		case 25: case 26: case 27: case 28: case 29: case 30: case 31:
		case 32: case 33: case 34: case 35: case 36: case 37: case 38:
		case 39:
		LE = 1;
		P1 = up | down;
		LE = 0;
	//	delay(1500);
		if(i==2 || i==9)
		{
			up = _cror_(up, 2);
			down = _crol_(down, 2);	
		}
		else
		{
			up = _cror_(up, 1);
			down = _crol_(down, 1);
		}break;
					}

	
	i ++;
	if(i > 39)
	{	i = 0;
	/*	init_led();	*/   }

}

void task_led1()
{
	static uint i;
	
//	for(i=0; i<8; i++)
	{
		init_led();
	//	delay(1000);
		switch(i)
		{
			case 0: LE = 1; P1 = 0x0f; LE = 0; 
					LE0 = 1; P0 = 0xf0; LE0 = 0; 
					LE1 = 1; P0 = 0xf0; LE1 = 0;
					LE2 = 1; P0 = 0xf0; LE2 = 0;
					LE3 = 1; P0 = 0xf0; LE3 = 0;
					break;
			case 1: LE = 1; P1 = 0x0f; LE = 0; 
					LE0 = 1; P0 = 0x0f; LE0 = 0; 
					LE1 = 1; P0 = 0x0f; LE1 = 0;
					LE2 = 1; P0 = 0x0f; LE2 = 0;
					LE3 = 1; P0 = 0x0f; LE3 = 0;
					break;
			case 2: LE = 1; P1 = 0x0f; LE = 0; 
					LE4 = 1; P0 = 0xf0; LE4 = 0; 
					LE5 = 1; P0 = 0xf0; LE5 = 0;
					LE6 = 1; P0 = 0xf0; LE6 = 0;
					LE7 = 1; P0 = 0xf0; LE7 = 0;
					break;
			case 3: LE = 1; P1 = 0x0f; LE = 0; 
					LE4 = 1; P0 = 0x0f; LE4 = 0; 
					LE5 = 1; P0 = 0x0f; LE5 = 0;
					LE6 = 1; P0 = 0x0f; LE6 = 0;
					LE7 = 1; P0 = 0x0f; LE7 = 0;
					break;
			case 4: LE = 1; P1 = 0xf0; LE = 0; 
					LE4 = 1; P0 = 0xf0; LE4 = 0; 
					LE5 = 1; P0 = 0xf0; LE5 = 0;
					LE6 = 1; P0 = 0xf0; LE6 = 0;
					LE7 = 1; P0 = 0xf0; LE7 = 0;
					break;
			case 5: LE = 1; P1 = 0xf0; LE = 0; 
					LE0 = 1; P0 = 0xf0; LE0 = 0; 
					LE1 = 1; P0 = 0xf0; LE1 = 0;
					LE2 = 1; P0 = 0xf0; LE2 = 0;
					LE3 = 1; P0 = 0xf0; LE3 = 0;
					break;
			case 6: LE = 1; P1 = 0xf0; LE = 0; 
					LE0 = 1; P0 = 0x0f; LE0 = 0; 
					LE1 = 1; P0 = 0x0f; LE1 = 0;
					LE2 = 1; P0 = 0x0f; LE2 = 0;
					LE3 = 1; P0 = 0x0f; LE3 = 0;
					break;
			case 7: LE = 1; P1 = 0xf0; LE = 0; 
					LE4 = 1; P0 = 0x0f; LE4 = 0; 
					LE5 = 1; P0 = 0x0f; LE5 = 0;
					LE6 = 1; P0 = 0x0f; LE6 = 0;
					LE7 = 1; P0 = 0x0f; LE7 = 0;
					break;
			
		}

		i++;
		if(i > 7)
			i = 0;
	//	delay(3000);
	}		
}

void task_led2()
{
	uint i=0;

	LE = 1;
	P1 = 0xff;
	LE = 0;

	P0 = 0xfe;
	for(i=0; i<8; i++)
	{
		LE0 = 1;
		LE0 = 0;
		LE1 = 1;
		LE1 = 0;
		LE2 = 1;
		LE2 = 0;
		LE3 = 1;
		LE3 = 0;
		LE4 = 1;
		LE4 = 0;
		LE5 = 1;
		LE5 = 0;
		LE6 = 1;
		LE6 = 0;
		LE7 = 1;
		LE7 = 0;
		P0 = _crol_(P0, 1);
		delay(3000);
	}		
}

void task_led3()
{
	uint i;
	uint up = 0x80;
	uint down = 0x01;

	LE = 1;
	P1 = 0x00;
	LE = 0;

	LE0 = 1;
	P0 = 0x00;
	LE0 = 0;

	LE1 = 1;
	P0 = 0x00;
	LE1 = 0;

	LE2 = 1;
	P0 = 0x00;
	LE2 = 0;

	LE3 = 1;
	P0 = 0x00;
	LE3 = 0;

	LE4 = 1;
	P0 = 0x00;
	LE4 = 0;

	LE5 = 1;
	P0 = 0x00;
	LE5 = 0;

	LE6 = 1;
	P0 = 0x00;
	LE6 = 0;

	LE7 = 1;
	P0 = 0x00;
	LE7 = 0;

	for(i=0; i<14; i++)
	{
		LE = 1;
		P1 = up | down;
		LE = 0;
		delay(1500);
		if(i==2 || i==9)
		{
			up = _cror_(up, 2);
			down = _crol_(down, 2);	
		}
		else
		{
			up = _cror_(up, 1);
			down = _crol_(down, 1);
		}
		
		delay(1500);
	}
}

void task_ledb()
{

}

void main()
{
	init_led();
	LE = 1;
					P1 = 0x00;
					LE = 0;
				
					LE0 = 1;
					P0 = 0x00;
					LE0 = 0;
				
					LE1 = 1;
					P0 = 0x00;
					LE1 = 0;
				
					LE2 = 1;
					P0 = 0x00;
					LE2 = 0;
				
					LE3 = 1;
					P0 = 0x00;
					LE3 = 0;
				
					LE4 = 1;
					P0 = 0x00;
					LE4 = 0;
				
					LE5 = 1;
					P0 = 0x00;
					LE5 = 0;
				
					LE6 = 1;
					P0 = 0x00;
					LE6 = 0;
				
					LE7 = 1;
					P0 = 0x00;
					LE7 = 0;
	init_timer0();
	uart_init();
	while(1)
	{	
		switch(flag)
		{
			case 0x00:	LE = 1;
						P1 = 0x00;
						LE = 0;break;
			case 0x01:  LE = 1;
						P1 = 0xff;
	                	LE = 0;break;
			case 0x02: break;
			default:   break;
		}		 
		//task_led1();
		

	}
}
