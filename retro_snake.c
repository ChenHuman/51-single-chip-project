//15通信 陈晓明  贪吃蛇游戏
#include <stdio.h>			
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

void gotoxy(int x,int y);  
void gamepicture();
void draw_snake();
void init();
void move_snake();
void draw_apple();
void gameover();
void File_out();
void File_in();
void color(int b);

int score[3]={0};		//score[0]表示目前得分 score[1]表示历史记录 score[2]用于速度调节	
int snake_x[21*21]={0};	//蛇x坐标
int snake_y[21*21]={0};		//蛇y坐标
int x[21*21]={0};			//蛇x坐标备份
int y[21*21]={0};			//蛇y坐标备份
int snake_length=3;			
char ch;
int flag1=0;				//有相应键按下才开始游戏的标志
int still=1;				//排除其他按键的干扰的标志
int apple[3]={0};			//apple[0]表示苹果x坐标 apple[1]表示苹果y坐标 apple[2]用于判断是否有苹果生成
int end=0;
int f=0;
int begin=0;				//确保一开始按键只能按蛇头可走方向进行

void gameover()					//判断游戏是否结束
{
	int i=0;
	if(snake_x[0]<1||snake_x[0]>21*2||snake_y[0]<1||snake_y[0]>21)
		end=1;	
	for(i=1;i<snake_length;i++)
		if(snake_x[0]==snake_x[i]&&snake_y[0]==snake_y[i])
			end=1;
	if(end)
	{
		File_out();
		gotoxy(23*2+7+14,11);
		color(12);
		printf("%d",score[1]);
	}		
}

void color(int b)				//文字颜色
{
	HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h,b);
}

void gotoxy(int x,int y)		//光标位置
{
	COORD pos;
	pos.X=x;
	pos.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

void gamepicture()				//游戏界面的设计
{
	int plate[23][23]={0};
	int i,j;
	for(i=1;i<=21;i++)
		for(j=1;j<=21;j++)
			plate[i][j]=1;
	for(i=0;i<23;i++)
	{
		gotoxy(0,i);
		for(j=0;j<23;j++)
			if(plate[i][j])
			{
				color(11);
				printf("■");
			}
			else
			{
				color(12);
				printf("□");
			}
	}

	gotoxy(23*2+7,2);
	color(10);
	printf("按 W S A D 移动方向");

	gotoxy(23*2+7,4);
	color(10);
	printf("按 W S A D 开始或继续游戏");
	
	gotoxy(23*2+7,6);
	color(10);
	printf("按 space 键暂停游戏");

	gotoxy(23*2+7,8);
	color(10);
	printf("按 Esc 键结束游戏");

	gotoxy(23*2+7,11);
	color(11);
	printf("历史最高分为: ");

	gotoxy(23*2+7+14,11);
	color(12);
	printf("%d",score[1]);

	gotoxy(23*2+7,16);
	color(11);
	printf("你现在得分为: ");

	gotoxy(23*2+7+14,16);
	color(12);
	printf("%d",score[0]);
}

void draw_snake()			//画蛇以及判断蛇是否吃到苹果
{
	int i=0;	
	if(still)
	{
		for(i=0;i<snake_length;i++)
		{
			gotoxy(snake_x[i],snake_y[i]);
			color(14);
			if(i==0)
				printf("●");
			else
				printf("★");
		}
		if(flag1)
		{	gotoxy(x[snake_length-1],y[snake_length-1]);
			color(11);
			printf("■");
		}
	}
	if(apple[0]*2==snake_x[0]&&apple[1]==snake_y[0])
	{
		apple[2]=1;
		score[0]++;
		gotoxy(23*2+7+14,16);
		color(12);
		printf("%d",score[0]);
		snake_length+=2;
		if(snake_length%5==0&&snake_length<=15)
			score[2]=score[2]+5;
	}
	if(snake_length>=5)
	{
		gotoxy(0,0);
		color(12);
		printf("□");
	}
	draw_apple();
}

void move_snake()			//控制蛇移动
{
	int i=0;
	while(1)
	{
N:		memcpy(x,snake_x,sizeof(int)*snake_length);
		memcpy(y,snake_y,sizeof(int)*snake_length);
		if(kbhit())
		{
			flag1=1;
			switch(ch=getch())
			{
				case 'w':
				case 'W':
					still=1;
					begin=1;
					if(f!=2)
					{
						snake_y[0]-=1;
						f=1;
						break;
					}
					else
						goto N;
				case 's':
				case 'S':
					still=1;
					begin=1;
					if(f!=1)
					{
						snake_y[0]+=1;
						f=2;
						break;
					}
					else 
						goto N;
				case 'a':
				case 'A':	
					still=1;
					begin=1;
					if(f!=4)
					{
						snake_x[0]-=2;
						f=3;
						break;
					}
					else 
						goto N;
				case 'd':
				case 'D':	
					still=1;
					if(f!=3&&begin)
					{
						snake_x[0]+=2;	
						f=4;
						break;
					}
					else 
						goto N;
				case 32:
					still=0;
					goto N;
				default:
					goto N;
			}
			if(still)
			{
				for(i=0;i<snake_length;i++)
				{
					snake_x[i+1]=x[i];
					snake_y[i+1]=y[i];
				}
				draw_snake();
			}
		}
		else 
		{
			if(still)
			{
				if(begin)
				{
					if(flag1)
					{
						if(f==1)
							snake_y[0]-=1;
						else if(f==2)
							snake_y[0]+=1;
						else if(f==3)
							snake_x[0]-=2;
						else if(f==4)
							snake_x[0]+=2;
						for(i=0;i<snake_length;i++)
						{
							snake_x[i+1]=x[i];
							snake_y[i+1]=y[i];
						}
						draw_snake();
					}
				}
			}
		}
		break;
	}
}

void draw_apple()		//随机生成苹果
{
	int i,j=1;
	if(apple[2])
	{
		while(1)
		{
			j=1;
			apple[0]=rand()%21+1;
			apple[1]=rand()%21+1;
			for(i=0;i<snake_length;i++)
				if(apple[0]*2==snake_x[i]&&apple[1]==snake_y[i])
					j=0;
			if(j)
			{
				gotoxy(apple[0]*2,apple[1]);
				color(12);
				printf("●");
				apple[2]=0;
				break;
			}
		}
	}
}

void init()				//初始化游戏主界面
{
	system("title 贪吃蛇");
	gamepicture();
	snake_x[0]=8;
	snake_y[0]=8;
	snake_x[1]=10;
	snake_y[1]=8;
	snake_x[2]=12;
	snake_y[2]=8;
	gotoxy(0,23);
	color(8);
	srand(time(NULL));
	apple[2]=1;
	score[2]=0;
	draw_snake();
	draw_apple();
	File_in();
	File_out();
	gotoxy(23*2+7+14,11);
	color(12);
	printf("%d",score[1]);
}

void File_in()     //取记录的分数 
 {
    FILE *fp;
    if((fp = fopen("C:\\tcs.txt","a+")) == NULL)
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
		if((fp = fopen("C:\\tcs.txt","w+")) == NULL)
		{
			printf("文件不能打开\n");
			exit(0);
		}
		fputc((char)score[0],fp);
		fclose(fp);
	}
 }

void main()
{
	init();
	while(ch!=0x1B)
	{
		if(!end)
		{
			move_snake();
			gameover();
		}
		else 
		{
			ch=0x1B;
			gotoxy(53,22);
			if(score[0]>score[1])	
			{	
				color(3);
				printf("恭喜您打破了纪录！\n");
			}
			else 
			{
				color(15);
				printf("很遗憾您没能打破纪录！\n");
			}
		}
		Sleep(200-score[2]*10);
	}
	gotoxy(0,23);
	while(1);
}