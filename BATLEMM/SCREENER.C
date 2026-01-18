#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <dos.h>
#include "rotate.c"


void setvideomode(unsigned int mode)
{
union REGS regs;

regs.x.ax=mode;
int86(0x10,&regs,&regs);
}

void rotate(void)
{
float new_x,new_y;
FILE *fp;
char fly[1600];
char inp;
int angle;
int x,y;
long pause;
int x_pos, y_pos;
int speed;
float how_high;

for(angle=0;angle<360;angle++)
{
sin_table[angle]=sin(angle*3.14159/180);
cos_table[angle]=cos(angle*3.14159/180);
}

fp=fopen("pause.grf","rb");
fread(&fly,1,1600,fp);
fclose(fp);

x=0;
y=0;

angle=0;
new_x=9;
new_y=23;

x_pos=150;
y_pos=100;
speed=0;

inp=getch();

speed=rand()%10;

while(!(kbhit()))
{



for(new_y=0;new_y<40;new_y++)
    {
    for(new_x=0;new_x<40;new_x++)
	{
	x=(int)((new_x-20)*cos_table[angle]-(new_y-20)*sin_table[angle]);
	y=(int)((new_y-20)*cos_table[angle]+(new_x-20)*sin_table[angle]);
	video_mem[22560+x+y*320]=fly[(int)(new_x)*40+(int)(40-new_y)];
	}
    }

angle+=speed;

if(angle<0)
    angle=359;
if(angle>359)
    angle=0;

}
KeyScan=0;
}

main()
{
char inp;
long num;

for(num=-15;num<20;num++)
{
setvideomode((unsigned int)num);
while(!(kbhit()))
    {
    printf("%i mode \r",num);
    video_mem[rand()%64000]=rand()%255;
    }
inp=getch();
}
setvideomode(19);
rotate();
setvideomode(7);
}
