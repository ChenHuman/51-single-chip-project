//15通信 陈晓明  俄罗斯方块游戏
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>

#define rotate  0
#define left    1
#define right   2
#define down    3
#define sink    4
#define quit    5
#define show    1		//显示方块
#define clear   2		//擦除方块
#define fix     3		//固定方块

int block_type[19][4][4]={      //19种俄罗斯方块形状
								{{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},  					
								{{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}},	
								{{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},  
								{{0,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,0,0}},  
								{{0,0,0,0},{0,0,1,0},{1,1,1,0},{0,0,0,0}},  
								{{0,0,0,0},{0,1,1,0},{0,0,1,0},{0,0,1,0}},			
								{{0,0,0,0},{0,1,1,1},{0,1,0,0},{0,0,0,0}},
								{{0,0,1,0},{0,0,1,0},{0,1,1,0},{0,0,0,0}},
								{{0,0,0,0},{1,1,1,0},{0,0,1,0},{0,0,0,0}},
								{{0,0,0,0},{0,1,1,0},{0,1,0,0},{0,1,0,0}},
								{{0,0,0,0},{0,1,0,0},{0,1,1,1},{0,0,0,0}},
								{{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},
								{{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
								{{0,0,0,0},{0,0,1,1},{0,1,1,0},{0,0,0,0}},
								{{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}},
								{{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
								{{0,1,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}},
								{{0,0,0,0},{1,1,1,0},{0,1,0,0},{0,0,0,0}},
								{{0,1,0,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}}
							};
int curwhich=0;					//当前方块
int nextwhich=0;				//下一方块
int flag=0;						//游戏结束标记
int zuobiao[46][23]={0};		//游戏区域内俄罗斯方块的坐标
char c;							//按键字符
int score[3]={0};				//score[0]表示当前得分 score[1]表示记录得分 score[2]用于下降速度调节
int cur_block_x=0;				//当前x坐标
int cur_block_y=0;				//当前y坐标
int next_block_x=0;				//同上
int next_block_y=0;				//同上
clock_t oldtime;				//设置下降时间间隔
clock_t newtime;

void onrotate();
void onleft();
void onright();
void ondown();
void onsink();
void gamepicture();
void gotoxy(int x,int y);
void color(int b);
void init();
void newgame();
void newblock();
void drawblock(int which,int _block_x,int _block_y,int type);
void drawunit(int x,int y,int type);
char getkey();
void dispatchkey();
int checkblock(int which,int _block_x,int _block_y);
void File_in();
void File_out();

void newgame()										//新游戏开始
{
	nextwhich=rand()%19;
	next_block_x=50;
	next_block_y=1 ;

	newblock();
}

void newblock()											//产生下一个方块
{	
	drawblock(nextwhich,next_block_x,next_block_y,clear);

	curwhich=nextwhich,nextwhich=rand()%19;
	cur_block_x=30;
	cur_block_y=1;

	drawblock(curwhich,cur_block_x,cur_block_y,show);
	drawblock(nextwhich,next_block_x,next_block_y,show);

	oldtime=clock();
}

void drawblock(int which,int _block_x,int _block_y,int type)			//画组合方块
{
	int x,y;
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			if(block_type[which][i][j])
			{
				x=_block_x+j*2;
				y=_block_y+i;
				if(y<23)
					drawunit(x,y,type);
			}
}

void drawunit(int x,int y,int type)				//画小方块
{
	switch(type)
	{
		case show:
			gotoxy(x,y);
			color(14);
			printf("■");
			break;
		case clear:
			gotoxy(x,y);
			color(11);
			printf("□");
			break;
		case fix:
			gotoxy(x,y);
			color(13);
			printf("■");
			break;
	}
}

char getkey()								//获取按键
{
	while(1)
	{
		newtime=clock();
		if(newtime-oldtime>=500-score[2]*60)
		{
			oldtime=newtime;
			return down;
		}
		if (kbhit())
		{
			switch(getch())
			{
				case 'w':
				case 'W':	return rotate;
				case 'a':
				case 'A':	return left;
				case 'd':
				case 'D':	return right;
				case 's':
				case 'S':	return down;
				case 27:	return quit;
				case ' ':	return sink;
				case 0:
				case 0xE0:					
					switch(getch())
					{
						case 72:	return rotate;
						case 75:	return left;
						case 77:	return right;
						case 80:	return down;
					}
			}
		}
		// 延时 (降低 CPU 占用率)
		Sleep(20);
	}
}

void init()
{
	srand((unsigned)time(NULL));			//产生随时间变化的随机种子
	gamepicture();							//画游戏主界面
	newgame();
	File_in();								
	File_out();
	gotoxy(50+14,8);
	color(13);
	printf("%d",score[1]);
	gotoxy(0,23);
	color(7);
}

void main()
{
	init();									
	while(1)
	{
		c=getkey();							//获取按键
		dispatchkey(c);						//分配按键，同时限制了按键种类
		if(flag)							//结束界面
		{
			gotoxy(50,22);
			color(7);
			if(flag==1)
				printf("很遗憾您失败了！");
			if(flag==2)
				printf("游戏结束！");
			File_out();
			gotoxy(50+14,8);
			color(13);
			printf("%d",score[1]);
			gotoxy(0,23);
			color(7);
			break;
		}
	}
}

void dispatchkey(char a)					//分配按键，同时限制了按键种类
{
	switch(a)
	{
		case rotate:   onrotate();  break;
		case left:     onleft();    break;
		case right:    onright();   break;
		case down:     ondown();    break;
		case sink:     onsink();    break;
		case quit:     flag=2;      break;
	}
}

void onrotate()								//方块变形函数
{
	int which=curwhich;
	int bx=cur_block_x;
	int by=cur_block_y;
	switch(which)							//不同方块对应不同变形
	{
		case 0:  which=1;  break;
		case 1:  which=0;  break;
		case 2:  which=2;  break;
		case 3:  which=4;  break;
		case 4:  which=5;  break;
		case 5:  which=6;  break;
		case 6:  which=3;  break;
		case 7:  which=8;  break;
		case 8:  which=9;  break;
		case 9:  which=10; break;
		case 10: which=7;  break;
		case 11: which=12; break;
		case 12: which=11; break;
		case 13: which=14; break;
		case 14: which=13; break;
		case 15: which=16; break;
		case 16: which=17; break;
		case 17: which=18; break;
		case 18: which=15; break;
	}
	if (checkblock(which,bx,by))	
	{
		drawblock(curwhich,cur_block_x,cur_block_y,clear);
		curwhich=which;
		drawblock(curwhich,cur_block_x,cur_block_y,show);
	}
}

void ondown()							//向下
{
	int bx=cur_block_x;
	int by=cur_block_y;
	by++;
	if(checkblock(curwhich,bx,by))
	{
		drawblock(curwhich,cur_block_x,cur_block_y,clear);      
		cur_block_y++;
		drawblock(curwhich,cur_block_x,cur_block_y,show);
	}
	else
		onsink();	// 不可下移时，执行“沉底方块”操作
}

void onleft()							//向左
{
	int bx=cur_block_x;
	int by=cur_block_y;
	bx=bx-2;
	if(checkblock(curwhich,bx,by))
	{
		drawblock(curwhich,cur_block_x,cur_block_y,clear);
		cur_block_x-=2;
		drawblock(curwhich,cur_block_x,cur_block_y,show);
	}
}

void onright()							//向右
{
	int bx=cur_block_x;
	int by=cur_block_y;
	bx=bx+2;
	if(checkblock(curwhich,bx,by))
	{
		drawblock(curwhich,cur_block_x,cur_block_y,clear);
		cur_block_x+=2;
		drawblock(curwhich,cur_block_x,cur_block_y,show);
	}
}

void onsink()								//沉降
{
	int remove=0;
	int bx=cur_block_x;
	int by=cur_block_y;
	int i,j;
	int x,y;
	int fy=0;

	drawblock(curwhich,cur_block_x,cur_block_y,clear);
	by++;
	while(checkblock(curwhich,bx,by))
	{
		cur_block_y++;
		by++;
	}
	drawblock(curwhich,cur_block_x,cur_block_y,fix);
	
	remove=0;fy=0;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			if(block_type[curwhich][i][j])
			{
				if(cur_block_y+i<=6)
					flag=1;
				else
					zuobiao[cur_block_x+j*2][cur_block_y+i]=1;
			}

	for(y=21;y>1;y--)
	{
		i=0;
		for(x=24;x<=42;x=x+2)
			if(zuobiao[x][y])
				i++;
		if(i==10)
		{
			remove++;
			gotoxy(24,y);
			color(159);
			for(j=0;j<10;j++)
				printf("★");
			for(x=24;x<=42;x=x+2)
				zuobiao[x][y]=0;
			fy=y;
			score[0]++;
			if(score[0]%5==0&&score[0]<=30)
				score[2]++;
			gotoxy(50+14,12);
			color(13);
			printf("%d",score[0]);
		}
	}

	if(remove)
	{
		for(y=fy;y>1;y--)
			for(x=24;x<=42;x=x+2)
				zuobiao[x][y+remove-1]=zuobiao[x][y-1];

		Sleep(300);

		for(y=21;y>1;y--)
			for(x=24;x<=42;x=x+2)
				if(zuobiao[x][y])
					drawunit(x,y,fix);
				else
					drawunit(x,y,clear);
	}

	newblock();	
}

int checkblock(int which,int _block_x,int _block_y)  //条件判断
{
	int i,j,x,y;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			if(block_type[which][i][j])
			{
				x=_block_x+j*2;
				y=_block_y+i;
				if(x<24||x>42||y>21)
					return 0;
				if(y>1&&zuobiao[x][y])
					return 0;
				if(zuobiao[x][6]==1)
					flag=1;
			}
	return 1;
}

void gamepicture()					//画游戏主界面
{
	int i,j;
	for(i=0;i<21;i++)
	{
		gotoxy(24,i+1);
		for(j=0;j<10;j++)
		{
			color(11);
			printf("□");
		}
	}

	for(i=0;i<4;i++)
	{
		gotoxy(50,i+1);
		for(j=0;j<4;j++)
		{
			color(11);
			printf("□");
		}
	}

	gotoxy(0,9);
	color(11);
	printf("操作说明");
	gotoxy(0,11);
	color(10);
	printf("上/W：旋转");
	gotoxy(0,13);
	color(10);
	printf("左/A：左移");
	gotoxy(0,15);
	color(10);
	printf("右/D：右移");
	gotoxy(0,17);
	color(10);
	printf("下/S：下移");
	gotoxy(0,19);
	color(10);
	printf("空格：沉底");
	gotoxy(0,21);
	color(10);
	printf("ESC：退出");

	gotoxy(50,8);
	color(11);
	printf("历史最高分为: ");
	gotoxy(50+14,8);
	color(13);
	printf("%d",score[1]);
	gotoxy(50,12);
	color(10);
	printf("你现在得分为: ");
	gotoxy(50+14,12);
	color(13);
	printf("%d",score[0]);

	gotoxy(50,16);
	color(158);
	printf("注意：                ");
	gotoxy(50,17);
	color(158);
	printf("    触及红线则游戏结束");

	gotoxy(0,6);
	color(12);
	printf("------------------------");
	gotoxy(44,6);
	printf("------------------------------------");
}

void gotoxy(int x, int y)    //输出坐标 
{
	COORD pos;
	pos.X = x; 
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void color(int b)         //颜色函数 
{
    HANDLE hConsole = GetStdHandle((STD_OUTPUT_HANDLE)) ; 
    SetConsoleTextAttribute(hConsole,b) ;
} 

void File_in()     //取记录的分数 
 {
    FILE *fp;
    if((fp = fopen("C:\\tetris.txt","a+")) == NULL)
    {
		printf("文件不能打开\n");
        exit(0);
    }
    if(((char)score[1] = fgetc(fp)) != EOF);
    else
    score[1] = 0;
	fclose(fp);
 }

void File_out()    //存数据 
 {    
    FILE *fp;
	if(score[0]>score[1])
	{
		if((fp = fopen("C:\\tetris.txt","w+")) == NULL)
		{
			printf("文件不能打开\n");
			exit(0);
		}
		fputc((char)score[0],fp);
		fclose(fp);
	}
 }
