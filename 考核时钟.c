// 15ͨ�� ������ 51��Ƭ��ʱ����Ŀ
#include <reg51.h>

#define uchar unsigned char
#define uint unsigned int

//�궨��ָ���ĸ�������ֵ
#define	key_int0_value 1
#define key_t0_value   2
#define key_s4_value   3
#define key_s5_value   4

//λ���������� ����ܶ�ѡ��λѡ �ĸ�����
sbit beep=P2^2;
sbit wela=P2^7;
sbit dula=P2^6;
sbit key_int0=P3^2;
sbit key_t0=P3^4;
sbit key_s4=P3^6;
sbit key_s5=P3^7;

//�����λ������ʾֵ
uchar code we_table[]=
{
    0xff,
	~0x01,~0x02,~0x04,~0x08,
	~0x10,~0x20,~0x40,~0x80
}; 
uchar code du_table[]=
{
    0x3f,0x06,0x5b,0x4f,
	0x66,0x6d,0x7d,0x07,
	0x7f,0x6f,0x77,0x7c,
	0x39,0x5e,0x79,0x71
};

//ȫ�ֱ�������
uchar key_value,key_press,mode,flash_time,flash_which,set_which,flash_shi,flash_fen,flash_miao,alarm_on,alarm_on_flag,beep_on;
uchar s4_s5_allow,count_allow,t0_allow,int0_allow,t3_clear_flag,miaobiao_on;
char shi,fen,miao,ashi,afen,amiao,start_reset_flag,pause_continue_flag,mode3_flag;
int t0,t1,t2,t3,t4,miao0,miao1,miao2,du_num,xianhou_flag;

void init();						 //��ʼ��
void t0_display();					 //�������ʾ
void t1_shijian();					 //ʱ��仯
void t2_keyscan();					 //����ɨ��
void dispatch_key();				 //���䰴���������Ӧ��������
void set_plus();					 //������ʱ���������ͣ�ͼ����Լ��������ӹ��ܺ͹رշ�����		  �����ܵ��ھ�����
void set_minus();					 //������ʱ������Ŀ�ʼ�͸�λ�Ͳ��������ӹ���		 �����ܵ��ھ�����
void flash();						 //�������˸
void t3_alarm_beep();				 //��������
void miaobiao();					 //���仯

void init()
{
	//��ʼ����ʱ��0
	TMOD=0x01;
	TH0=(65536-1000)/256;
	TL0=(65536-1000)%256;
	EA=1; ET0=1; TR0=1;

	//ʱ�ӡ����ӡ�����ֵ
	shi=23;	 fen=59;  miao=55;
	ashi=0;	 afen=0;  amiao=0;
	miao0=0; miao1=0; miao2=0;

	//��Ҫ��־λ��ֵ
	count_allow=1;	 //����ʱ���߶���־
	int0_allow=1;	 //����int0���������µı�־
	alarm_on_flag=1; //Ĭ�Ͽ������ӹ���
	alarm_on=0;		 //Ĭ���������켴����������

	/*ע�⣺alarm_on_flag��alarm_on��ͬ��ǰ�߱�ʾ�Ƿ������ӹ��ܣ����߱�ʾ�Ƿ��������Ӽ���������
	���ƣ����������ʱ����ֻ�رշ����������ر����ӹ��ܣ�����һ��ʱ�䵽���Ի���*/
}

void main()
{
	init();
	while(1)
	{
		if(t0==2) {t0=0; t0_display();}	

		if(t1==965)
		{
			t1=0;
			if(count_allow)
				t1_shijian();
		}

		if(t2==5) {t2=0; t2_keyscan();}

		if(shi==ashi&&fen==afen&&miao==amiao&&alarm_on)
			beep_on=1;
	}
}

void timer0() interrupt 1
{
	TH0=(65536-1000)/256;
	TL0=(65536-1000)%256;
	t0++;
	t1++;
	t2++;
	if(beep_on)
	{
		t3++;
		t3_alarm_beep();
	}
	if(mode==3&&miaobiao_on)
	{
		t4++;
		if(t4==1)
		{
			t4=0;
			miaobiao();
		}
	}
}

void t0_display()
{
	static uchar we_num=1;
	
	P0=we_table[we_num];
	wela=1;
	wela=0;
	
	P0=0x00;
	dula=1;
	dula=0;
	
	switch(mode)
	{
		case 0:
		case 1:
			switch(we_num)
			{
				case 1: du_num=shi/10;  break; 
				case 2:	du_num=shi%10;  break;
				case 4:	du_num=fen/10;  break;
				case 5:	du_num=fen%10;  break;
				case 7:	du_num=miao/10; break;
				case 8:	du_num=miao%10; break;
			}
			break;
		case 2:
			switch(we_num)
			{
				case 1: du_num=ashi/10;  break; 
				case 2:	du_num=ashi%10;  break;
				case 4:	du_num=afen/10;  break;
				case 5:	du_num=afen%10;  break;
				case 7:	du_num=amiao/10; break;
				case 8:	du_num=amiao%10; break;
			}
			break;
		case 3:
			switch(we_num)
			{
				case 1: du_num=miao0/10;       break; 
				case 2:	du_num=miao0%10;       break;
				case 4:	du_num=miao1/10;       break;
				case 5:	du_num=miao1%10;       break;
				case 7: du_num=miao2/100;      break;
			} 
			break;
	}	
	
	if(flash_miao) flash_which=flash_miao;
	else if(flash_fen)  flash_which=flash_fen;
	else if(flash_shi)  flash_which=flash_shi;
	else flash_which=0;

	if(mode!=3)
	switch(flash_which)
	{
		case 0:
			P0=du_table[du_num];
			dula=1;
			dula=0;
			break;
		case 1:
			if(we_num==8)
				flash();
			else
			{
				P0=du_table[du_num];
				dula=1;
				dula=0;
			}
			break;
		case 2:
			if(we_num==5)
				flash();
			else
			{
				P0=du_table[du_num];
				dula=1;
				dula=0;
			}
			break;
		case 3:
			if(we_num==2)
				flash();
			else
			{
				P0=du_table[du_num];
				dula=1;
				dula=0;
			}
			break;
	}
	else
	{
		P0=du_table[du_num];
		dula=1;
		dula=0;
	}

	we_num++;

	if(we_num==3||we_num==6)
		we_num++;
		
	if(mode==3)
	{	
		if(we_num>7)
			we_num=1;
	}
	else
	{
		if(we_num>8)
			we_num=1;
	}		
}

void t1_shijian()
{
	miao++;
	if(miao>59)
	{
		miao=0;
		fen++;
		if(fen>59)
		{
			fen=0;
			shi++;
			if(shi>23)
				shi=0;
		}
	}
}

void t2_keyscan()
{
	static uchar state=0;

	if(key_int0==0)  		key_value=key_int0_value;
	else if(key_t0==0)		key_value=key_t0_value;
	else if(key_s4==0)		key_value=key_s4_value;
	else if(key_s5==0)		key_value=key_s5_value;
	else					key_value=0;

	switch(state)
	{
		case 0: if(key_value!=0) state=1; break;
		case 1: state=2; break;
		case 2: if(key_value!=0) {key_press=key_value; state=3;}
				else state=0;  break;
		case 3:	if(key_value==0) {dispatch_key();state=0;} break;
	}
}

void t3_alarm_beep()
{
	if(t3==500)
		if(alarm_on)
			beep=0;
	if(t3==1000)
	{
		t3=0;
		beep=1;
	}	
}

void miaobiao()
{
	miao2++;
	if(miao2>999)
	{
		miao2=0;
		miao1++;
		if(miao1>59)
		{
			miao1=0;
			miao0=1;
			if(miao0>59)
				miao0=0;
		}
	}
}

void flash()
{
	flash_time++;
	if(flash_time>28)
	{
		P0=du_table[du_num];
		dula=1;
		dula=0;
	}	
	if(flash_time>56)
		flash_time=0;
}

void set_plus()
{
	if(s4_s5_allow||mode3_flag)
	{
		switch(mode)
		{
			case 0:
				break;
			case 1:
				if(s4_s5_allow)
					switch(set_which)
					{
						case 1:	miao++;	if(miao>59)	miao=0;	break;	
						case 2:	fen++;	if(fen>59)	fen=0;	break;
						case 3:	shi++;	if(shi>23)	shi=0;	break;
					}
				break;
			case 2:
				if(s4_s5_allow)
					switch(set_which)
					{
						case 1:	amiao++; if(amiao>59) amiao=0; break;	
						case 2:	afen++;	 if(afen>59)  afen=0;  break;
						case 3:	ashi++;	 if(ashi>23)  ashi=0;  break;
					}
				break;	
			case 3:
				if(xianhou_flag)
				{
					if(pause_continue_flag==0)
					{
						miaobiao_on=0;
						pause_continue_flag=1;
					}
					else
					{
						miaobiao_on=1;
						pause_continue_flag=0;
					}
				}
				break;
		}
	}	
}

void set_minus()
{
	if(s4_s5_allow||mode3_flag)
	{
		switch(mode)
		{
			case 0:
				break;
			case 1:
				if(s4_s5_allow)
					switch(set_which)
					{
						case 1:	miao--;	if(miao<0)	miao=59;  break;	
						case 2:	fen--;	if(fen<0)	fen=59;	  break;
						case 3:	shi--;	if(shi<0)	shi=23;	  break;
					}
				break;
			case 2:
				if(s4_s5_allow)
					switch(set_which)
					{
						case 1:	amiao--; if(amiao<0)  amiao=59;	 break;	
						case 2:	afen--;	 if(afen<0)	  afen=59;	 break;
						case 3:	ashi--;	 if(ashi<0)	  ashi=23;   break;
					}
				break;	
			case 3:
				if(start_reset_flag==0)
				{
					t4=0;
					miaobiao_on=1;
					start_reset_flag=1;
					xianhou_flag=1;
				}
				else
				{
					t4=0;
					miao0=0;
					miao1=0;
					miao2=0;
					miaobiao_on=0;
					start_reset_flag=0;
					xianhou_flag=0;
				}
				break;
		}
	}	
}

void dispatch_key()
{
	switch(key_press)
	{
		case key_int0_value:
			if(!beep_on)
				if(int0_allow)
				{	
					alarm_on_flag=0;
					s4_s5_allow=0;
					t0_allow=1;
					mode++;
					mode3_flag=1;
					switch(mode)
					{
						case 1:	break;	
						case 2:	break;
						case 3:	break;
						case 4:	mode=0;	alarm_on_flag=1; t0_allow=0; break;
					}
				}	
			break;
		case key_t0_value:
			if(mode!=3)
				if(!beep_on)
					if(t0_allow)
					{
						if(mode==1)	count_allow=0;
						else if(mode==2) count_allow=1;
						else if(mode==3) count_allow=1;

						s4_s5_allow=1;
						int0_allow=0;
						set_which++;
						flash_which++;

						switch(set_which)
						{
							case 1:	flash_miao=1;	break;
							case 2:	flash_fen=2;	flash_miao=0;	break;
							case 3:	flash_shi=3;	flash_fen=0;	break;
							case 4:
								alarm_on_flag=1;
								flash_shi=0;
								set_which=0;
								count_allow=1;
								flash_which=0;
								int0_allow=1;
								TH0=(65536-1000)/256;
								TL0=(65536-1000)%256;
								mode=0;
								s4_s5_allow=0;
								t0_allow=0;
								break;
						}
					}
			break;
		case key_s4_value:
			set_minus();
			if(alarm_on_flag&&!beep_on)
				alarm_on=0;
			break;
		case key_s5_value:
			set_plus();
			if(alarm_on_flag&&!beep_on)
				alarm_on=1;
			if(beep_on)
			{	
				beep_on=0;
				beep=1;
				t3=0;
			}
			break;
	}
}

