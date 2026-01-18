#include <stdio.h>
#include <stdlib.h>
#include <graph.h>
#include <conio.h>
#include <malloc.h>
#include <dos.h>
#include <time.h>
#include "keys.c"
#include "palette.fct"
#include "rotate.c"
#include "sound.c"
//#include "animate.c"

#define NUM_LINES	 120
#define TIME_SPEED	 9
#define SOUND_SPEED	 10
#define PALETTE_MASK	 0x3c6
#define PALETTE_REGISTER 0x3c8
#define PALETTE_DATA	 0x3c9
#define PAUSE_KEY	 153

/////////////////////////////////////////////////////////////////////////////
//video_mem=(char far *)0xA0000000L;
int PrevMode;
char lem[2000];
char bad_lemi[5][2000];
long how_far;
int curr_pal[768];
FILE *fp;
char mug[12696];
char numbers[1440];
char far *sounds[5];
unsigned char lengths[5];
char sound_num;
int sound_timer;
int sound_dist;
int map_x,map_y;
/////////////////////////////////////////////////////////////////////////////
typedef struct
{
char far *line;
}scroll_lines;

typedef struct
{
char  missle_graph[4][25];
int   missle_pos[4][2];
char  missle_speed[2];
char  expl_graph[500];
char  expl_frame;
int   expl_pos[2];
char  missle_start_sp[2];
char  missle_fall;
char  missle_range;
}missle_typ;

typedef struct
{
char dir,frame,action,jump,fall;
int x,y;
char dead_alive;
char *sprite_graph;
char back_up;
}sprite_typ;

typedef struct
{
sprite_typ *its_me;
sprite_typ *kill_him;
missle_typ *dodge_this;
char move_or_not;
char to_or_from;
char shoot_or_not;
char jump_or_not;
char dodge_or_not;
int how_far_to;
int how_far_from;
char keep_dir;
long timer;
char command;
}brain_typ;

scroll_lines data[NUM_LINES];

typedef struct
{
FILE *level_graph;
FILE *char_graphs[5];
char main_char_gr;
FILE *proj_graph[5];
FILE *main_proj_gr;
FILE *expl_graph[5];
//FILE *main_expl_gr;
//FILE *level_title;
int start_seconds,
    start_minutes,
    start_hours;
}level_typ;
/////////////////////////////////////////////////////////////////////////////
level_typ level;
sprite_typ lemming;
missle_typ green_ball;
sprite_typ bad_lem[5];
missle_typ blue_ball[5];
brain_typ brains[5];
/////////////////////////////////////////////////////////////////////////////
void set_color(int index, char red, char green, char blue)
{
outp(PALETTE_MASK,0xff);
outp(PALETTE_REGISTER,index);
outp(PALETTE_DATA,red);
outp(PALETTE_DATA,green);
outp(PALETTE_DATA,blue);
}

/////////////////////////////////////////////////////////////////////////////
animate(char *filename, int num_frames)
{
char inp;
int x,y,frame;
char gr[30000];

//gr=(char *)malloc((unsigned int)30000);

//setvideomode(19);

for(x=0;x<256;x++)
    set_color(x,pal[x*3+0],pal[x*3+1],pal[x*3+2]);

fp=fopen(filename,"rb");
//printf("\r a");

for(frame=0;frame<num_frames;frame++)
{
//printf("\r                      1");
fread(&gr,1,30000,fp);
//printf("\r                      2");
for(y=0;y<100;y++)
//    for(x=0;x<200;x++)
//	video_mem[x+y*320]=graph[y*200+x];
    _fmemcpy((char far *)&video_mem[y*320+16010],(char far *)&gr[y*300],300);
//printf("\r                      3");
//inp=getch();
}

printf("\r That is all. \n");
fclose(fp);

//free(gr);

//setvideomode(3);
}

/////////////////////////////////////////////////////////////////////////////
void check_hit(sprite_typ *obj_me, missle_typ *obj_this,char which)
{
sprite_typ its_me;
missle_typ dodge_this;
long num,num2;

its_me	   = *obj_me;
dodge_this = *obj_this;

num  = (dodge_this.missle_pos[0][0]+2) - (its_me.x+5);
num2 = (dodge_this.missle_pos[0][1]+2) - (its_me.y+5);
if(num < 0)
    num *= -1;
if(num2< 0)
    num2*= -1;

if(its_me.dead_alive)
    {
    set_color(255-which,85+rand()%3,0,0);
    curr_pal[255-which+0]=85+rand()%3;
    curr_pal[255-which+1]=0;
    curr_pal[255-which+2]=0;
    }
if(num<=dodge_this.missle_range && num2<=dodge_this.missle_range && dodge_this.missle_speed[0])
    {
	its_me.back_up=0;
	set_color(255-which,15,15,15);
	curr_pal[255-which+0]=15;
	curr_pal[255-which+1]=15;
	curr_pal[255-which+2]=15;
	fp=fopen("lem_dead.grf","rb");
	fread(its_me.sprite_graph,1,400,fp);
	fclose(fp);
	its_me.frame=0;
	its_me.action=0;
	its_me.jump=0;
	dodge_this.expl_pos[0]=dodge_this.missle_pos[0][0]-3;
	dodge_this.expl_pos[1]=dodge_this.missle_pos[0][1]-5;
	dodge_this.expl_frame=0;
	dodge_this.missle_speed[0]=0;
	Voc_Stop_Sound();
	Voc_Play_Sound((char far *)sounds[0],lengths[0]);
    }
its_me.dead_alive=its_me.back_up;
*obj_me=its_me;
*obj_this=dodge_this;
}

/////////////////////////////////////////////////////////////////////////////

char use_brains(brain_typ *think_o)
{
long num;
long num2;
brain_typ think;

think=*think_o;

think.command=0;

think.timer++;
num=rand()%100;

if(think.timer>=10)
{
think.timer=0;

if(num < think.to_or_from)
{
if((think.kill_him->x) >= (think.its_me->x))
    think.command=think.command | 0x10;

if((think.kill_him->x) <= (think.its_me->x))
    think.command=think.command | 0x80;
}

if(num > think.to_or_from)
{
if((think.kill_him->x) <= (think.its_me->x))
    think.command=think.command | 0x10;
if((think.kill_him->x) >= (think.its_me->x))
    think.command=think.command | 0x80;
}

if(rand()%100 > think.move_or_not)
    think.command=think.command & 0x0F;

think.keep_dir=think.command & 0xF0;
}
if(think.its_me->dead_alive)
if(rand()%100 < think.shoot_or_not)
    {
    if(think.kill_him->x < think.its_me->x)
	think.its_me->dir=1;
    else
	think.its_me->dir=0;
    think.command=think.command | 0x04;
    }

if(rand()%100 < think.jump_or_not && data[think.its_me->y].line[think.its_me->x+10-10*think.its_me->dir]!=0)
    think.command=think.command | 0x08;

num=(think.its_me->x)-(think.kill_him->x);
if(num<0)
    num*=-1;

//if(think->how_far_from > num)
//    if(think->kill_him->x < think->its_me->x)
//	{
//	think->command=think->keep_dir & 0x0F;
//	think->command=think->keep_dir | 0x10;
//	}
//    else
//	{
//	think->command=think->keep_dir & 0x0F;
//	think->command=think->keep_dir | 0x80;
//	}

//if(think->how_far_to < num)
//    if(think->kill_him->x > think->its_me->x)
//	{
//	think->command=think->keep_dir & 0x0F;
//	think->command=think->keep_dir | 0x10;
//	}
//    else
//	{
//	think->command=think->keep_dir & 0x0F;
//	think->command=think->keep_dir | 0x80;
//	}

num  = (think.dodge_this->missle_pos[0][0]+2) - (think.its_me->x+5);
num2 = (think.dodge_this->missle_pos[0][1]+2) - (think.its_me->y+5);
if(num<0)
    num*=-1;
if(num2<0)
    num2=-1;

if(num<=15 && num2<=10 && (rand()%100)<=(think.dodge_or_not) && think.dodge_this->missle_speed[0]!=0)
    think.command=think.command | 0x08;

if(num<=5 && num2<=5)
    think.its_me->dead_alive=0;

//if(think->command & 0x80)
//    video_mem[63680]=rand()%4+1;
//if(think->command & 0x40)
//    video_mem[63681]=rand()%4+1;
//if(think->command & 0x20)
//    video_mem[63682]=rand()%4+1;
//if(think->command & 0x10)
//    video_mem[63683]=rand()%4+1;
//if(think->command & 0x08)
//    video_mem[63684]=rand()%4+1;
//if(think->command & 0x04)
//    video_mem[63685]=rand()%4+1;
//if(think->command & 0x02)
//    video_mem[63686]=rand()%4+1;
//if(think->command & 0x01)
//    video_mem[63687]=rand()%4+1;

if(rand()%100<think.jump_or_not && data[think.its_me->y+15].line[think.its_me->x+12-14*think.its_me->dir]==0 && think.its_me->action==1)
    think.command=think.command | 0x08;

think.command=think.command | think.keep_dir;

*think_o=think;
return(think.command);
}

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
void move_missle(missle_typ *green_ball_o)
{

missle_typ green_ball;
green_ball=*green_ball_o;

if(green_ball.missle_speed[0])
{
green_ball.missle_pos[3][0]=green_ball.missle_pos[2][0];
green_ball.missle_pos[3][1]=green_ball.missle_pos[2][1];
green_ball.missle_pos[2][0]=green_ball.missle_pos[1][0];
green_ball.missle_pos[2][1]=green_ball.missle_pos[1][1];
green_ball.missle_pos[1][0]=green_ball.missle_pos[0][0];
green_ball.missle_pos[1][1]=green_ball.missle_pos[0][1];
green_ball.missle_pos[0][0]+=green_ball.missle_speed[0];
green_ball.missle_pos[0][1]+=green_ball.missle_speed[1];
}

if((data[green_ball.missle_pos[0][1]].line[green_ball.missle_pos[0][0]]) && green_ball.missle_speed[0]!=0)
    {
    if(green_ball.expl_frame>=5)
    {
    green_ball.expl_pos[0]=green_ball.missle_pos[0][0]-3;
    green_ball.expl_pos[1]=green_ball.missle_pos[0][1]-5;
    }
    green_ball.expl_frame=0;
    if(green_ball.missle_pos[0][0]>sound_dist-160 && green_ball.missle_pos[0][0]<sound_dist+160)
	sound_num=1;
    }

if((data[green_ball.missle_pos[0][1]].line[green_ball.missle_pos[0][0]]))
    green_ball.missle_speed[0]=0;

if(green_ball.expl_frame<5)
    green_ball.expl_frame++;

if(green_ball.missle_speed[0]!=0)
    green_ball.missle_speed[1]+=green_ball.missle_fall;

if(green_ball.missle_pos[0][0]>630 || green_ball.missle_pos[0][0]<10)
    green_ball.missle_speed[0] = 0;
if(green_ball.missle_pos[0][1]>110 || green_ball.missle_pos[0][1]<10)
    green_ball.missle_speed[0] = 0;

*green_ball_o=green_ball;
}
/////////////////////////////////////////////////////////////////////////////
void draw_expl(missle_typ *object_o)
{
int p,num,curr_pict;
missle_typ object;

object=*object_o;

for(num=0;num<10;num++)
{
//_fmemcpy(video_mem[16020+320num],lemming[num*10+frame*100],10);
//for(p=0;p<10;p++)
    data[(num+object.expl_pos[1])].line[object.expl_pos[0]+0]+=object.expl_graph[object.expl_frame*100+(num<<1)+(num<<3)+0];
    data[(num+object.expl_pos[1])].line[object.expl_pos[0]+1]+=object.expl_graph[object.expl_frame*100+(num<<1)+(num<<3)+1];
    data[(num+object.expl_pos[1])].line[object.expl_pos[0]+2]+=object.expl_graph[object.expl_frame*100+(num<<1)+(num<<3)+2];
    data[(num+object.expl_pos[1])].line[object.expl_pos[0]+3]+=object.expl_graph[object.expl_frame*100+(num<<1)+(num<<3)+3];
    data[(num+object.expl_pos[1])].line[object.expl_pos[0]+4]+=object.expl_graph[object.expl_frame*100+(num<<1)+(num<<3)+4];

    data[(num+object.expl_pos[1])].line[object.expl_pos[0]+5]+=object.expl_graph[object.expl_frame*100+(num<<1)+(num<<3)+5];
    data[(num+object.expl_pos[1])].line[object.expl_pos[0]+6]+=object.expl_graph[object.expl_frame*100+(num<<1)+(num<<3)+6];
    data[(num+object.expl_pos[1])].line[object.expl_pos[0]+7]+=object.expl_graph[object.expl_frame*100+(num<<1)+(num<<3)+7];
    data[(num+object.expl_pos[1])].line[object.expl_pos[0]+8]+=object.expl_graph[object.expl_frame*100+(num<<1)+(num<<3)+8];
    data[(num+object.expl_pos[1])].line[object.expl_pos[0]+9]+=object.expl_graph[object.expl_frame*100+(num<<1)+(num<<3)+9];
}

//*object_o=object;
}
/////////////////////////////////////////////////////////////////////////////
void undo_expl(missle_typ *object_o)
{
int p,num,curr_pict;
missle_typ object;

object=*object_o;

for(num=0;num<10;num++)
{
//_fmemcpy(video_mem[16020+320num],lemming[num*10+frame*100],10);
//for(p=0;p<10;p++)
    data[(num+object.expl_pos[1])].line[object.expl_pos[0]]-=object.expl_graph[object.expl_frame*100+num*10];
    data[(num+object.expl_pos[1])].line[object.expl_pos[0]+1]-=object.expl_graph[object.expl_frame*100+num*10+1];
    data[(num+object.expl_pos[1])].line[object.expl_pos[0]+2]-=object.expl_graph[object.expl_frame*100+num*10+2];
    data[(num+object.expl_pos[1])].line[object.expl_pos[0]+3]-=object.expl_graph[object.expl_frame*100+num*10+3];
    data[(num+object.expl_pos[1])].line[object.expl_pos[0]+4]-=object.expl_graph[object.expl_frame*100+num*10+4];

    data[(num+object.expl_pos[1])].line[object.expl_pos[0]+5]-=object.expl_graph[object.expl_frame*100+num*10+5];
    data[(num+object.expl_pos[1])].line[object.expl_pos[0]+6]-=object.expl_graph[object.expl_frame*100+num*10+6];
    data[(num+object.expl_pos[1])].line[object.expl_pos[0]+7]-=object.expl_graph[object.expl_frame*100+num*10+7];
    data[(num+object.expl_pos[1])].line[object.expl_pos[0]+8]-=object.expl_graph[object.expl_frame*100+num*10+8];
    data[(num+object.expl_pos[1])].line[object.expl_pos[0]+9]-=object.expl_graph[object.expl_frame*100+num*10+9];
}
}

/////////////////////////////////////////////////////////////////////////////
void draw_missle(missle_typ *object)
{
int p,num,curr_pict;
for(curr_pict=0;curr_pict<4;curr_pict++)
{
for(num=0;num<5;num++)
{
//_fmemcpy(video_mem[16020+320num],lemming[num*10+frame*100],10);
//for(p=0;p<5;p++)
    data[(num+object->missle_pos[curr_pict][1])].line[object->missle_pos[curr_pict][0]]+=object->missle_graph[curr_pict][num*5];
    data[(num+object->missle_pos[curr_pict][1])].line[object->missle_pos[curr_pict][0]+1]+=object->missle_graph[curr_pict][1+num*5];
    data[(num+object->missle_pos[curr_pict][1])].line[object->missle_pos[curr_pict][0]+2]+=object->missle_graph[curr_pict][2+num*5];
    data[(num+object->missle_pos[curr_pict][1])].line[object->missle_pos[curr_pict][0]+3]+=object->missle_graph[curr_pict][3+num*5];
    data[(num+object->missle_pos[curr_pict][1])].line[object->missle_pos[curr_pict][0]+4]+=object->missle_graph[curr_pict][4+num*5];
}
}
}
/////////////////////////////////////////////////////////////////////////////
void undo_missle(missle_typ *object)
{
int p,num,curr_pict;
for(curr_pict=0;curr_pict<4;curr_pict++)
{
for(num=0;num<5;num++)
{
//_fmemcpy(video_mem[16020+320num],lemming[num*10+frame*100],10);
//for(p=0;p<5;p++)
    data[(num+object->missle_pos[curr_pict][1])].line[object->missle_pos[curr_pict][0]]-=object->missle_graph[curr_pict][num*5];
    data[(num+object->missle_pos[curr_pict][1])].line[object->missle_pos[curr_pict][0]+1]-=object->missle_graph[curr_pict][num*5+1];
    data[(num+object->missle_pos[curr_pict][1])].line[object->missle_pos[curr_pict][0]+2]-=object->missle_graph[curr_pict][num*5+2];
    data[(num+object->missle_pos[curr_pict][1])].line[object->missle_pos[curr_pict][0]+3]-=object->missle_graph[curr_pict][num*5+3];
    data[(num+object->missle_pos[curr_pict][1])].line[object->missle_pos[curr_pict][0]+4]-=object->missle_graph[curr_pict][num*5+4];
}
}
}

/////////////////////////////////////////////////////////////////////////////
void draw_sprite(sprite_typ *object_o)
{
int p,num;
sprite_typ object;

object=*object_o;

if(object.dir==0)
{
for(num=0;num<10;num++)
{
//_fmemcpy(video_mem[16020+320num],lemming[num*10+frame*100],10);
//for(p=0;p<10;p++)
    data[(num+object.y)].line[object.x+0]+=object.sprite_graph[(num<<1)+(num<<3)+0+object.action*400+object.frame*100];
    data[(num+object.y)].line[object.x+1]+=object.sprite_graph[(num<<1)+(num<<3)+1+object.action*400+object.frame*100];
    data[(num+object.y)].line[object.x+2]+=object.sprite_graph[(num<<1)+(num<<3)+2+object.action*400+object.frame*100];
    data[(num+object.y)].line[object.x+3]+=object.sprite_graph[(num<<1)+(num<<3)+3+object.action*400+object.frame*100];
    data[(num+object.y)].line[object.x+4]+=object.sprite_graph[(num<<1)+(num<<3)+4+object.action*400+object.frame*100];

    data[(num+object.y)].line[object.x+5]+=object.sprite_graph[(num<<1)+(num<<3)+5+object.action*400+object.frame*100];
    data[(num+object.y)].line[object.x+6]+=object.sprite_graph[(num<<1)+(num<<3)+6+object.action*400+object.frame*100];
    data[(num+object.y)].line[object.x+7]+=object.sprite_graph[(num<<1)+(num<<3)+7+object.action*400+object.frame*100];
    data[(num+object.y)].line[object.x+8]+=object.sprite_graph[(num<<1)+(num<<3)+8+object.action*400+object.frame*100];
    data[(num+object.y)].line[object.x+9]+=object.sprite_graph[(num<<1)+(num<<3)+9+object.action*400+object.frame*100];
}
}
else
    {
    for(num=0;num<10;num++)
    {
    //_fmemcpy(video_mem[16020+320num],lemming[num*10+frame*100],10);
//    for(p=0;p<10;p++)
	data[(num+object.y)].line[object.x+0]+=object.sprite_graph[(num<<1)+(num<<3)+9+object.action*400+object.frame*100];
	data[(num+object.y)].line[object.x+1]+=object.sprite_graph[(num<<1)+(num<<3)+8+object.action*400+object.frame*100];
	data[(num+object.y)].line[object.x+2]+=object.sprite_graph[(num<<1)+(num<<3)+7+object.action*400+object.frame*100];
	data[(num+object.y)].line[object.x+3]+=object.sprite_graph[(num<<1)+(num<<3)+6+object.action*400+object.frame*100];
	data[(num+object.y)].line[object.x+4]+=object.sprite_graph[(num<<1)+(num<<3)+5+object.action*400+object.frame*100];

	data[(num+object.y)].line[object.x+5]+=object.sprite_graph[(num<<1)+(num<<3)+4+object.action*400+object.frame*100];
	data[(num+object.y)].line[object.x+6]+=object.sprite_graph[(num<<1)+(num<<3)+3+object.action*400+object.frame*100];
	data[(num+object.y)].line[object.x+7]+=object.sprite_graph[(num<<1)+(num<<3)+2+object.action*400+object.frame*100];
	data[(num+object.y)].line[object.x+8]+=object.sprite_graph[(num<<1)+(num<<3)+1+object.action*400+object.frame*100];
	data[(num+object.y)].line[object.x+9]+=object.sprite_graph[(num<<1)+(num<<3)+object.action*400+object.frame*100];
    }
    }
*object_o=object;
}
/////////////////////////////////////////////////////////////////////////////
void undo_sprite(sprite_typ *object_o)
{
int p,num;
sprite_typ object;
object=*object_o;

if(object.dir==0)
{
for(num=0;num<10;num++)
{
//_fmemcpy(video_mem[16020+320num],lemming[num*10+frame*100],10);
//for(p=0;p<10;p++)
//    data[(num+object.y)].line[object.x+p]-=object.sprite_graph[num*10+p+object.action*400+object.frame*100];
    data[(num+object.y)].line[object.x+0]-=object.sprite_graph[(num<<1)+(num<<3)+0+object.action*400+object.frame*100];
    data[(num+object.y)].line[object.x+1]-=object.sprite_graph[(num<<1)+(num<<3)+1+object.action*400+object.frame*100];
    data[(num+object.y)].line[object.x+2]-=object.sprite_graph[(num<<1)+(num<<3)+2+object.action*400+object.frame*100];
    data[(num+object.y)].line[object.x+3]-=object.sprite_graph[(num<<1)+(num<<3)+3+object.action*400+object.frame*100];
    data[(num+object.y)].line[object.x+4]-=object.sprite_graph[(num<<1)+(num<<3)+4+object.action*400+object.frame*100];

    data[(num+object.y)].line[object.x+5]-=object.sprite_graph[(num<<1)+(num<<3)+5+object.action*400+object.frame*100];
    data[(num+object.y)].line[object.x+6]-=object.sprite_graph[(num<<1)+(num<<3)+6+object.action*400+object.frame*100];
    data[(num+object.y)].line[object.x+7]-=object.sprite_graph[(num<<1)+(num<<3)+7+object.action*400+object.frame*100];
    data[(num+object.y)].line[object.x+8]-=object.sprite_graph[(num<<1)+(num<<3)+8+object.action*400+object.frame*100];
    data[(num+object.y)].line[object.x+9]-=object.sprite_graph[(num<<1)+(num<<3)+9+object.action*400+object.frame*100];

}
}
else
    {
    for(num=0;num<10;num++)
    {
    //_fmemcpy(video_mem[16020+320num],lemming[num*10+frame*100],10);
//    for(p=0;p<10;p++)
//	data[(num+object.y)].line[object.x+p]-=object.sprite_graph[num*10+(9-p)+object.action*400+object.frame*100];
	data[(num+object.y)].line[object.x+0]-=object.sprite_graph[(num<<1)+(num<<3)+9+object.action*400+object.frame*100];
	data[(num+object.y)].line[object.x+1]-=object.sprite_graph[(num<<1)+(num<<3)+8+object.action*400+object.frame*100];
	data[(num+object.y)].line[object.x+2]-=object.sprite_graph[(num<<1)+(num<<3)+7+object.action*400+object.frame*100];
	data[(num+object.y)].line[object.x+3]-=object.sprite_graph[(num<<1)+(num<<3)+6+object.action*400+object.frame*100];
	data[(num+object.y)].line[object.x+4]-=object.sprite_graph[(num<<1)+(num<<3)+5+object.action*400+object.frame*100];

	data[(num+object.y)].line[object.x+5]-=object.sprite_graph[(num<<1)+(num<<3)+4+object.action*400+object.frame*100];
	data[(num+object.y)].line[object.x+6]-=object.sprite_graph[(num<<1)+(num<<3)+3+object.action*400+object.frame*100];
	data[(num+object.y)].line[object.x+7]-=object.sprite_graph[(num<<1)+(num<<3)+2+object.action*400+object.frame*100];
	data[(num+object.y)].line[object.x+8]-=object.sprite_graph[(num<<1)+(num<<3)+1+object.action*400+object.frame*100];
	data[(num+object.y)].line[object.x+9]-=object.sprite_graph[(num<<1)+(num<<3)+object.action*400+object.frame*100];

    }
    }
*object_o=object;
}
/////////////////////////////////////////////////////////////////////////////
void draw_big_picture(int how_far)
{
int how_low,num;
for(how_low=0;how_low<NUM_LINES;how_low+=20)
    {
    _fmemcpy((char far *)&video_mem[((how_low+20)<<6)+((how_low+20)<<8)],(char far *)&data[how_low].line[how_far],320);
    _fmemcpy((char far *)&video_mem[((how_low+21)<<6)+((how_low+21)<<8)],(char far *)&data[how_low+1].line[how_far],320);
    _fmemcpy((char far *)&video_mem[((how_low+22)<<6)+((how_low+22)<<8)],(char far *)&data[how_low+2].line[how_far],320);
    _fmemcpy((char far *)&video_mem[((how_low+23)<<6)+((how_low+23)<<8)],(char far *)&data[how_low+3].line[how_far],320);
    _fmemcpy((char far *)&video_mem[((how_low+24)<<6)+((how_low+24)<<8)],(char far *)&data[how_low+4].line[how_far],320);

    _fmemcpy((char far *)&video_mem[((how_low+25)<<6)+((how_low+25)<<8)],(char far *)&data[how_low+5].line[how_far],320);
    _fmemcpy((char far *)&video_mem[((how_low+26)<<6)+((how_low+26)<<8)],(char far *)&data[how_low+6].line[how_far],320);
    _fmemcpy((char far *)&video_mem[((how_low+27)<<6)+((how_low+27)<<8)],(char far *)&data[how_low+7].line[how_far],320);
    _fmemcpy((char far *)&video_mem[((how_low+28)<<6)+((how_low+28)<<8)],(char far *)&data[how_low+8].line[how_far],320);
    _fmemcpy((char far *)&video_mem[((how_low+29)<<6)+((how_low+29)<<8)],(char far *)&data[how_low+9].line[how_far],320);

    _fmemcpy((char far *)&video_mem[((how_low+30)<<6)+((how_low+30)<<8)],(char far *)&data[how_low+10].line[how_far],320);
    _fmemcpy((char far *)&video_mem[((how_low+31)<<6)+((how_low+31)<<8)],(char far *)&data[how_low+11].line[how_far],320);
    _fmemcpy((char far *)&video_mem[((how_low+32)<<6)+((how_low+32)<<8)],(char far *)&data[how_low+12].line[how_far],320);
    _fmemcpy((char far *)&video_mem[((how_low+33)<<6)+((how_low+33)<<8)],(char far *)&data[how_low+13].line[how_far],320);
    _fmemcpy((char far *)&video_mem[((how_low+34)<<6)+((how_low+34)<<8)],(char far *)&data[how_low+14].line[how_far],320);

    _fmemcpy((char far *)&video_mem[((how_low+35)<<6)+((how_low+35)<<8)],(char far *)&data[how_low+15].line[how_far],320);
    _fmemcpy((char far *)&video_mem[((how_low+36)<<6)+((how_low+36)<<8)],(char far *)&data[how_low+16].line[how_far],320);
    _fmemcpy((char far *)&video_mem[((how_low+37)<<6)+((how_low+37)<<8)],(char far *)&data[how_low+17].line[how_far],320);
    _fmemcpy((char far *)&video_mem[((how_low+38)<<6)+((how_low+38)<<8)],(char far *)&data[how_low+18].line[how_far],320);
    _fmemcpy((char far *)&video_mem[((how_low+39)<<6)+((how_low+39)<<8)],(char far *)&data[how_low+19].line[how_far],320);

    }
}
/////////////////////////////////////////////////////////////////////////////
void setVGApalette(unsigned char *buffer)
{
union REGS reg;
struct SREGS inreg;

reg.x.ax=0x1012;
segread(&inreg);
inreg.es=inreg.ds;
reg.x.bx=0;
reg.x.cx=256;
reg.x.dx=(int)&buffer;
int86x(0x10,&reg,&reg,&inreg);
}
/////////////////////////////////////////////////////////////////////////////
void fade_in(char speed)
{
long pause,num;
int x,y;
for(x=0;x<255;x++)
    {
    for(pause=0;pause<speed*5;pause++)
	{}
    for(y=0;y<768;y++)
	if(curr_pal[y]<pal[y])
	    curr_pal[y]+=1;
    for(y=0;y<256;y++)
	set_color(y,curr_pal[y*3],curr_pal[y*3+1],curr_pal[y*3+2]);
    }
}

void fade_out(char speed)
{
long pause;
int x,y;
for(x=0;x<63;x++)
    {
    for(pause=0;pause<32000;pause++)
	{}
    for(y=0;y<768;y++)
	if(curr_pal[y]>0)
	    curr_pal[y]-=speed;
    for(y=0;y<256;y++)
	set_color(y,curr_pal[y*3],curr_pal[y*3+1],curr_pal[y*3+2]);
    }
}

/////////////////////////////////////////////////////////////////////////////
void setvideomode(unsigned int mode)
{
union REGS regs;

regs.x.ax=mode;
int86(0x10,&regs,&regs);
}
/////////////////////////////////////////////////////////////////////////////
void move_sprite(sprite_typ *lemming_o,missle_typ *green_ball_o, char input)
{
sprite_typ lemming;
missle_typ green_ball;
long num;

lemming=*lemming_o;
green_ball=*green_ball_o;

if((lemming.action)==4 && (lemming.frame)==3)
    (lemming.action)=0;
if((lemming.action)!=2 && lemming.action<5 && lemming.dead_alive)
    (lemming.frame)++;
if((lemming.action)==2)
    if(lemming.jump==lemming.jump-lemming.jump%3)
	(lemming.frame)++;
if((lemming.frame)>3 && (lemming.dead_alive))
    (lemming.frame)=0;
if(lemming.dead_alive==0 && lemming.frame<3)
    {
    lemming.action=0;
    lemming.frame++;
    }

how_far=lemming.x-160;
if((lemming.action)<4)
    (lemming.action)=0;
if((lemming.action)==4 && (lemming.frame)==4)
    (lemming.action)=0;

if(lemming.dead_alive==1)
{
if(input & 0x80)
    {
    lemming.dir=1;
    (lemming.action)=1;
    }
if(input & 0x10)
    {
    lemming.dir=0;
    (lemming.action)=1;
    }
if(data[lemming.y].line[lemming.x+10-10*lemming.dir]!=0 && lemming.jump==0)
    (lemming.action)=0;

if(((lemming.action)) && (input & 0x90))
    {
    if(data[lemming.y].line[lemming.x+10-10*lemming.dir]==0)
	lemming.x=lemming.x+2-4*lemming.dir;		      ///*******
    }
if((input & 0x08) && lemming.jump==0 && lemming.fall==0)
    {
    (lemming.frame)=0;
    (lemming.action)=2;
    lemming.jump=9;

//    if(ct_voice_status==0)
//    {
    if(lemming.x>sound_dist-160 && lemming.x<sound_dist+160);
	sound_num=3;
//    }
//else
//    {
//    Voc_Stop_Sound();
//    Voc_Play_Sound(sounds[3],lengths[3]);
//    }
    }
if((input & 0x04) && (lemming.action)==0 && green_ball.missle_speed[0]==0 && green_ball.expl_frame>=5 && lemming.jump==0 && lemming.fall==0)
    {
    (lemming.frame)=0;
    (lemming.action)=4;
    green_ball.missle_pos[0][0]=lemming.x+10-10*lemming.dir;
    green_ball.missle_pos[0][1]=lemming.y;
    green_ball.missle_pos[1][0]=lemming.x+10-10*lemming.dir;
    green_ball.missle_pos[1][1]=lemming.y;
    green_ball.missle_pos[2][0]=lemming.x+10-10*lemming.dir;
    green_ball.missle_pos[2][1]=lemming.y;
    green_ball.missle_pos[3][0]=lemming.x+10-10*lemming.dir;
    green_ball.missle_pos[3][1]=lemming.y;

    green_ball.missle_speed[0]=green_ball.missle_start_sp[0]-(green_ball.missle_start_sp[0]*2)*lemming.dir;
    green_ball.missle_speed[1]=green_ball.missle_start_sp[1];

if(lemming.x>=sound_dist && lemming.x<=sound_dist+320)
{
Voc_Stop_Sound();
Voc_Play_Sound((char far *)sounds[2],lengths[2]);
}

    }
}

if(how_far<0)
    how_far=0;
if(how_far>320)
    how_far=320;

if(data[lemming.y+10].line[lemming.x+5]!=0)
    lemming.fall=0;
for(num=0;num<=(lemming.jump-(lemming.jump%2))/2;num++)
    if(data[lemming.y-num].line[lemming.x+5]!=0)
	lemming.jump=0;

//if(lemming.jump==lemming.jump-(lemming.jump%2))
    lemming.y-=(lemming.jump-lemming.jump%2)/2;

if(lemming.jump)
    {
    (lemming.action)=2;
    lemming.jump--;
    }
if(data[lemming.y+10].line[lemming.x+5]==0 && lemming.jump==0)
{
num=0;
lemming.fall++;
while(data[lemming.y+10+num].line[lemming.x+5]==00 && num<lemming.fall)
    num++;


lemming.y+=num;
if(lemming.fall>2)
    (lemming.action)=3;
}
if(data[lemming.y+9].line[lemming.x+5]!=0 && lemming.jump==0)
    while(data[lemming.y+9].line[lemming.x+5]!=0)
	lemming.y--;
*lemming_o=lemming;
*green_ball_o=green_ball;

}
/////////////////////////////////////////////////////////////////////////////
void pause_game(void)
{

char red,green,blue;
int num;

for(num=0;num<256;num++)
    {
    red   = curr_pal[num*3+0];
    green = curr_pal[num*3+1];
    blue  = curr_pal[num*3+2];
    red   = red   >> 1;
    green = green >> 1;
    blue  = blue  >> 1;
    set_color(num,red,green,blue);
    }
while(KeyScan==PAUSE_KEY)
    {}
for(num=0;num<256;num++)
    set_color(num,curr_pal[num*3+0],curr_pal[num*3+1],curr_pal[num*3+2]);

}
/////////////////////////////////////////////////////////////////////////////
void play_level(void)
{
char how_low;
char inp;
long r_x,r_y;
char r_bak[6];
long exit_counter;


long num,num2,x,y;
unsigned char palette[768];
long some_number;
int hours,minutes,seconds;

printf("\n Free space to store the data.");



//fread(&numbers,1,1440,number_ptr);

for(num=0;num<5;num++)
    {
    brains[num].its_me=&bad_lem[num];
    brains[num].kill_him=&lemming;
    }

printf("\n Set the target for all enemies.");

for(num=0;num<5;num++)
    brains[num].dodge_this=&green_ball;

printf("\n Tell them what to dodge.");


for(some_number=0;some_number<NUM_LINES;some_number++)
     data[some_number].line=_fmalloc((unsigned)640);

printf("\n Free space for the level data.");

printf("\n The first line of level data is stored at %p. ",data);

for(num=0;num<NUM_LINES;num++)
{
if(data[num].line==NULL)
    {
    printf("\n D'OH!!! \n");
    printf("\n there is only enouph space for %i",num);
    goto here;
    }
}

printf("\n There is enough space for all of the lines.");

printf("\n try to open the file with the level data.");
if(!(level.level_graph))
    {
    printf("You can't use a file if it doesn't exist! \n");
    goto here;
    }

printf("\n Checked for existance of file.");

for(some_number=0;some_number<NUM_LINES;some_number++)
{
for(num=0;num<640;num++)
    data[some_number].line[num]=getc(level.level_graph);
}

printf("\n Load it up.");

fclose(level.level_graph);

printf("\n Close it \n");

fp=fopen("good.guy","rb");

fread(&lem,1,1600,fp);

for(x=0;x<=level.main_char_gr;x++)
    fread(&lem[1600],1,400,fp);

printf("\n Read the main sprite data.");

fclose(fp);

printf("\n Load up the sprite data.");

for(num=0;num<5;num++)
{
fread(&bad_lemi[num][0],1,2000,level.char_graphs[num]);
fclose(level.char_graphs[num]);
}

printf("\n Load up some more sprite data.");

lemming.sprite_graph=(char *)lem;
for(num=0;num<5;num++)
    bad_lem[num].sprite_graph=&bad_lemi[num][0];

printf("\n Set the pointers to the data.");

for(num=0;num<5;num++)
{
fread(&blue_ball[num].missle_graph[0][0],1,25,level.proj_graph[num]);
fread(&blue_ball[num].missle_graph[1][0],1,25,level.proj_graph[num]);
fread(&blue_ball[num].missle_graph[2][0],1,25,level.proj_graph[num]);
fread(&blue_ball[num].missle_graph[3][0],1,25,level.proj_graph[num]);
fclose(level.proj_graph[num]);
}

printf("\n Load the projectile data.");

fread(&green_ball.missle_graph[0][0],1,25,level.main_proj_gr);
fread(&green_ball.missle_graph[1][0],1,25,level.main_proj_gr);
fread(&green_ball.missle_graph[2][0],1,25,level.main_proj_gr);
fread(&green_ball.missle_graph[3][0],1,25,level.main_proj_gr);
fclose(level.main_proj_gr);

printf("\n Same thing for the good guy.");

//fread(&green_ball.expl_graph,1,500,level.main_expl_gr);

//green_ball.missle_graph[0]=(char *)fire;
//green_ball.missle_graph[1]=(char *)fire[25];
//green_ball.missle_graph[2]=(char *)fire[50];
//green_ball.missle_graph[3]=(char *)fire[75];

printf("\n \n \n     !!!SEE YA!!!");

setvideomode(19);
fade_out(1);

//fp=fopen("pipemaze.grf","rb");
//for(num=0;num<64000;num++)
//    video_mem[num]=getc(fp);//level.level_title);
//fclose(fp);//level.level_title);
//fade_in(1);

set_key_driver();

printf("\n Keyboard driver set!");

//KeyScan=0;


//while(!(KeyScan))
//{}

how_far=0;

inp=0;

//fade_out(1);

fp=fopen("top.grf","rb");
for(num=0;num<6400;num++)
    video_mem[num]=getc(fp);
fclose(fp);
fp=fopen("bottom.grf","rb");
for(num=0;num<19200;num++)
    video_mem[44800+num]=getc(fp);
fclose(fp);
draw_sprite(&lemming);
for(num=0;num<5;num++)
    draw_sprite(&bad_lem[num]);
draw_big_picture(how_far);
fade_in(1);
undo_sprite(&lemming);
for(num=0;num<5;num++)
    undo_sprite(&bad_lem[num]);

// 0 - right, 1+ - left//////////////////////////////////////////////////////

seconds = level.start_seconds;
minutes = level.start_minutes;
hours	=   level.start_hours;

for(y=0;y<36;y++)
    for(x=0;x<105;x++)
	video_mem[x+(y+2)*320+48208]=data[y*3].line[x*6];

exit_counter=0;

sound_timer=0;

while(KeyScan!=129)
{
for(num=0;num<5;num++)
    if(bad_lem[num].dead_alive)
	video_mem[num]=5;
    else
	video_mem[num]=0;
//printf("\r 1  ");
sound_dist=how_far;

//printf("\r 2  ");

//sound_timer++;
//if(sound_timer>=SOUND_SPEED)
//    {
//    sound_timer=0;
//    printf("\r 1 of 2  ");
//    if(sound_num!=200)
    if(sound_num>=sound_timer || sound_timer==200)
    {
    sound_timer=sound_num;
//    printf("\r 2 of 2  ");
    if(sound_timer<200)
    {
//    Voc_Stop_Sound();
//    printf("\r 3 of 2  ");
//    Voc_Play_Sound(sounds[sound_num],lengths[sound_num]);
    sound_num=200;
    sound_timer=200;
    }
//    printf("\r 4 of 2  ");
    }
if(ct_voice_status==0)
    {
    sound_num=200;
    sound_timer=200;
    }
//    sound_num=200;
//    }
//printf("\r 3  ");

//if(!(ct_voice_status))
//    Voc_Stop_Sound();

inp=0;
if(green_ball.expl_frame<5)
    draw_expl(&green_ball);


if(blue_ball[0].expl_frame<5)
    draw_expl(&blue_ball[0]);
if(blue_ball[1].expl_frame<5)
    draw_expl(&blue_ball[1]);
if(blue_ball[2].expl_frame<5)
    draw_expl(&blue_ball[2]);
if(blue_ball[3].expl_frame<5)
    draw_expl(&blue_ball[3]);
if(blue_ball[4].expl_frame<5)
    draw_expl(&blue_ball[4]);


if(green_ball.missle_speed[0])
    draw_missle(&green_ball);

//for(num=0;num<5;num++)
if(blue_ball[0].missle_speed[0])
    draw_missle(&blue_ball[0]);
if(blue_ball[1].missle_speed[0])
    draw_missle(&blue_ball[1]);
if(blue_ball[2].missle_speed[0])
    draw_missle(&blue_ball[2]);
if(blue_ball[3].missle_speed[0])
    draw_missle(&blue_ball[3]);
if(blue_ball[4].missle_speed[0])
    draw_missle(&blue_ball[4]);


draw_sprite(&lemming);


draw_sprite(&bad_lem[0]);
draw_sprite(&bad_lem[1]);
draw_sprite(&bad_lem[2]);
draw_sprite(&bad_lem[3]);
draw_sprite(&bad_lem[4]);


draw_big_picture(how_far);

undo_sprite(&lemming);


undo_sprite(&bad_lem[0]);
undo_sprite(&bad_lem[1]);
undo_sprite(&bad_lem[2]);
undo_sprite(&bad_lem[3]);
undo_sprite(&bad_lem[4]);

if(green_ball.expl_frame<5)
    undo_expl(&green_ball);

for(num=0;num<5;num++)
    if(blue_ball[num].expl_frame<5)
    undo_expl(&blue_ball[num]);

if(green_ball.missle_speed[0])
    undo_missle(&green_ball);

for(num=0;num<5;num++)
    if(blue_ball[num].missle_speed[0])
    undo_missle(&blue_ball[num]);

/////////////////////////////////////////////////////////////////////////////

for(num=0;num<5;num++)
    move_missle(&blue_ball[num]);

for(num=0;num<5;num++)
    {
    inp=use_brains(&brains[num]);
    move_sprite(&bad_lem[num],&blue_ball[num],inp);
    }


if((!(bad_lem[0].dead_alive)) && (!(video_mem[320])))
    video_mem[322]=1;


move_missle(&green_ball);
move_sprite(&lemming,&green_ball,keys);

//for(x=0;x<5;x++)
//    {
//    r_y=(bad_lem[x].y+3-(bad_lem[x].y+3)%3)/3+2;
//    r_x=(bad_lem[x].x+8-(bad_lem[x].x+8)%6)/6-2;
//    video_mem[48210+r_x+r_y*320]=15;
//    }

//    r_y=(lemming.y+3-(lemming.y+3)%3)/3+2;
//    r_x=(lemming.x+8-(lemming.x+8)%3)/6-2;
//    video_mem[48210+r_x+r_y*320]=5;

for(num=0;num<5;num++)
if(lemming.dead_alive)
    check_hit(&lemming,&blue_ball[num],6);

for(num=0;num<5;num++)
for(num2=0;num2<5;num2++)
//if(bad_lem[num].dead_alive)
//    check_hit(bad_lem[num],blue_ball[num2]);
for(num=0;num<5;num++)
    check_hit(&bad_lem[num],&green_ball,num);



if(lemming.dead_alive)
{
for(num=0;num<46;num++)
    {
    //_fmemcpy(video_mem[16020+320num],lemming[num*10+frame*100],10);
	_fmemcpy((char far *)&video_mem[47047+(num<<6)+(num<<8)],(char far *)&mug[num*46+lemming.action*2116],46);
    }
}
else
    {
for(num=0;num<46;num++)
    {
    _fmemcpy((char far *)&video_mem[47047+num*320],(char far *)&mug[num*46+10580],46);
    //_fmemcpy(video_mem[16020+320num],lemming[num*10+frame*100],10);
    }
}

//for(num=0;num<46;num++)
//    {
//    for(num2=0;num2<46;num++)
//	{
//	video_mem[47047+num2320+num]=0;
//	video_mem[47047+num2320+num]+=mug[lemming.action*2116+num+num246];
//	}
//    }

if(seconds>=0)
    seconds-=TIME_SPEED;

num=0;

if(minutes)
    num++;
if(seconds)
    num++;
if(num)
    {
    if(seconds<=0)
	if(minutes)
	{
	seconds=59;
	minutes--;
	}
    if(minutes<=0)
	{

	if(hours)
	    {
	    hours--;
	    minutes=59;
	    }
	}
    }
if(seconds<0)
    seconds=0;

//if(seconds<0 && minutes<0 && hours<0)
//    {
//    seconds=0;
//    minutes=0;
//    hours=0;
//    }


for(y=0;y<12;y++)
	_fmemcpy((char far *)&video_mem[47770+y*320],(char far *)&numbers[y*12+((hours-(hours%100))/100)*144],12);
	//video_mem[47770+x+y*320]=numbers[x+y*12+((hours-(hours%100))/100)*144];
for(y=0;y<12;y++)
	_fmemcpy((char far *)&video_mem[47770+12+y*320],(char far *)&numbers[y*12+((hours%100)-(hours%10))/10144],12);
	//video_mem[47770+12+x+y*320]=numbers[x+y*12+((hours%100)-(hours%10))/10144];
for(y=0;y<12;y++)
	_fmemcpy((char far *)&video_mem[47770+24+y*320],(char far *)&numbers[y*12+((hours%10))*144],12);
	//video_mem[47770+24+x+y*320]=numbers[x+y*12+((hours%10))*144];

for(y=0;y<12;y++)
	_fmemcpy((char far *)&video_mem[47770+42+y*320],(char far *)&numbers[y*12+((minutes%100)-(minutes%10))/10144],12);
	//video_mem[47770+42+x+y*320]=numbers[x+y*12+((minutes%100)-(minutes%10))/10144];
for(y=0;y<12;y++)
	_fmemcpy((char far *)&video_mem[47770+54+y*320],(char far *)&numbers[y*12+((minutes%10))*144],12);
	//video_mem[47770+54+x+y*320]=numbers[x+y*12+((minutes%10))*144];

for(y=0;y<12;y++)
	_fmemcpy((char far *)&video_mem[47770+72+y*320],(char far *)&numbers[y*12+((seconds%100)-(seconds%10))/10144],12);
	//video_mem[47770+72+x+y*320]=numbers[x+y*12+((seconds%100)-(seconds%10))/10144];
for(y=0;y<12;y++)
	_fmemcpy((char far *)&video_mem[47770+84+y*320],(char far *)&numbers[y*12+((seconds%10))*144],12);
	//video_mem[47770+84+x+y*320]=numbers[x+y*12+((seconds%10))*144];

if(KeyScan==PAUSE_KEY)
    pause_game();
/////////////////////////////////////////////////////////////////////////////




if(lemming.dead_alive==0)
    exit_counter++;
if(bad_lem[0].dead_alive==0 && bad_lem[1].dead_alive==0 && bad_lem[2].dead_alive==0 && bad_lem[3].dead_alive==0 && bad_lem[4].dead_alive==0)
    exit_counter++;

if(exit_counter>100)
    KeyScan=129;

if(ct_voice_status)
    video_mem[10]=5;
else
    video_mem[10]=0;

//for(x=0;x<5;x++)
//    video_mem[48210+r_x+r_y*320]=0;

//    video_mem[48210+r_x+r_y*320]=0;

}

for(num=0;num<NUM_LINES;num++)
    _ffree(data[num].line);

//fade_out(1);

here:;
restore_key_driver();
}
/////////////////////////////////////////////////////////////////////////////
void view_picture(char *file_name,char typ_in, char typ_out)
{
char far *double_buffer;
char inp;
int how_high[320];
char how_fast[320];
long num,num2;
FILE *pic;
int x,y;

if(typ_in==1)
{
fade_out(1);
pic=fopen(file_name,"rb");
for(num=0;num<64000;num++)
    video_mem[num]=getc(pic);
fclose(pic);
fade_in(1);
}

if(typ_in==2)
{
double_buffer=(char far *)_fmalloc((unsigned int)64000);
pic=fopen(file_name,"rb");
for(num=0;num<64000;num++)
    double_buffer[num]=getc(pic);
fclose(pic);
for(num=0;num<400000;num++)
    {
//    num2=rand()%64000;
    x=rand()%320;
    y=rand()%200;
    video_mem[x+(y<<6)+(y<<8)]=double_buffer[x+(y<<6)+(y<<8)];
    }
_fmemcpy((char far *)&video_mem[0],(char far *)&double_buffer[0],(unsigned int)64000);
_ffree(double_buffer);
}

if(typ_in==3)
{
double_buffer=(char far *)_fmalloc((unsigned int)64000);
pic=fopen(file_name,"rb");
for(num=0;num<64000;num++)
    double_buffer[num]=getc(pic);
fclose(pic);
for(num=0;num<320;num++)
    {
//    y=rand()%10+5;
//    for(x=0;x<5;x++)
//{
    how_high[num]=0;
    how_fast[num]=rand()%10+15;
//    }
    }
for(num=0;num<14;num++)
{
for(num2=0;num2<320;num2++)
    {
    how_high[num2]+=how_fast[num2];
    if(how_high[num2]>200)
	how_high[num2]=200;
    }
for(num2=0;num2<320;num2++)
    {
for(x=0;x<how_high[num2];x++)
    {
    y=200-how_high[num2]+x;
    video_mem[num2+(x<<8)+(x<<6)]=double_buffer[num2+(y<<6)+(y<<8)];
    }
}
}
_ffree(double_buffer);
}

if(typ_in==4)
{
double_buffer=(char far *)_fmalloc((unsigned int)64000);
pic=fopen(file_name,"rb");
for(num=0;num<64000;num++)
    double_buffer[num]=getc(pic);
fclose(pic);


for(y=0;y<200;y+=3)
    _fmemcpy((char far *)&video_mem[64000-y*320],(char far *)&double_buffer[0],y*320);
_ffree(double_buffer);
}

if(typ_in==5)
{
double_buffer=(char far *)_fmalloc((unsigned int)64000);
pic=fopen(file_name,"rb");
for(num=0;num<64000;num++)
    double_buffer[num]=getc(pic);
fclose(pic);

for(x=0;x<320;x+=11)
{
for(y=0;y<100;y++)
{
_fmemcpy((char far *)&video_mem[y*640],(char far *)&double_buffer[y*640+319-x],x);
_fmemcpy((char far *)&video_mem[y*640+639-x],(char far *)&double_buffer[y*640+320],x);
}
}
_ffree(double_buffer);
}

if(typ_in==6)
{
double_buffer=(char far *)_fmalloc((unsigned int)64000);
pic=fopen(file_name,"rb");
for(num=0;num<64000;num++)
    double_buffer[num]=getc(pic);
fclose(pic);
for(num=0;num<320;num++)
    {
//    y=rand()%10+5;
//    for(x=0;x<5;x++)
//{
    how_high[num]=0;
    how_fast[num]=rand()%10+15;
//    }
    }
for(num=0;num<14;num++)
{
for(num2=0;num2<320;num2++)
    {
    how_high[num2]+=how_fast[num2];
    if(how_high[num2]>200)
	how_high[num2]=200;
    }
for(x=0;x<how_high[num2];x++)
    {
for(num2=0;num2<320;num2++)
{
    y=200-how_high[num2]+x;
    video_mem[num2+(x<<8)+(x<<6)]=double_buffer[num2+(y<<6)+(y<<8)];
    }
}
}
_ffree(double_buffer);
}


//printf("\n Press any key to continue.\n");
if(typ_out)
inp=getch();
}





//level_typ level,
//sprite_typ lemming,
//missle_typ green_ball,
//sprite_typ bad_lem[5],
//missle_typ blue_ball[5],
//brain_typ brains[5]

main()
{

int num;
char inp;
char char_num;
char pict[10000];
int y,x;


//for(num=0;num<768;num++)
//    pal[num]=pal[num]*4;

setvideomode(19);

Voc_Load_Driver();
Voc_Init_Driver();
Voc_Set_Port(0x220);
Voc_Set_DMA(5);
Voc_Set_Status_Addr((char far *)&ct_voice_status);
Voc_Set_Speaker(1);
sounds[2]=Voc_Load_Sound("woosh.voc", &lengths[2]);
//sounds[3]=Voc_Load_Sound("jump.voc" , &lengths[3]);
//sounds[1]=Voc_Load_Sound("wabam.voc" , &lengths[1]);
sounds[0]=Voc_Load_Sound("ouch.voc" , &lengths[0]);

fade_out(1);
fade_in(3);

//animate("intr1.anm",16);

sounds[4]=Voc_Load_Sound("song1.voc",&lengths[4]);
Voc_Play_Sound(sounds[4],lengths[4]);
view_picture("intr1.grf",1,3);
view_picture("intr2.grf",3,4);
view_picture("intr3.grf",2,1);
view_picture("intr4.grf",4,3);
Voc_Stop_Sound();
Voc_Unload_Sound(sounds[4]);
sounds[4]=Voc_Load_Sound("title.voc",&lengths[4]);
Voc_Play_Sound(sounds[4],lengths[4]);
view_picture("intr5.grf",5,2);
Voc_Stop_Sound();
Voc_Unload_Sound(sounds[4]);

sounds[4]=Voc_Load_Sound("vvzz.voc",&lengths[4]);
view_picture("charslct.grf",3,0);
char_num=0;

fcloseall();

if((kbhit()))
    inp=getch();

inp=45;

while(inp!=' ')
{
//fp=fopen("charctrs.grf","rb");
//if(!(fp))
//   printf("\r No character shots! \r");
//for(num=0;num<=char_num;num++)
//    for(x=0;x<10000;x++)
//	pict[x]=getc(fp);
//    fread(&pict,1,10000,fp);
//fclose(fp);

//num=0;

//while(num<1000000 && (!(kbhit())))
//{
//num++;
//x=rand()%100;
//y=rand()%100;

//if(pict[x+y*100])
//    video_mem[32110+x+y*320]=pict[x+y*100];
//}
x=rand()%255;

video_mem[12802+level.main_char_gr*45]=x;
video_mem[12803+level.main_char_gr*45]=x;
video_mem[13122+level.main_char_gr*45]=x;

video_mem[12845+level.main_char_gr*45]=x;
video_mem[12846+level.main_char_gr*45]=x;
video_mem[13166+level.main_char_gr*45]=x;

video_mem[27202-320+level.main_char_gr*45]=x;
video_mem[27203-320+level.main_char_gr*45]=x;
video_mem[26882-320+level.main_char_gr*45]=x;

video_mem[27246-320+level.main_char_gr*45]=x;
video_mem[27245-320+level.main_char_gr*45]=x;
video_mem[26926-320+level.main_char_gr*45]=x;

inp=0;
if(kbhit())
    inp=getch();
if(inp)
{
Voc_Stop_Sound();
Voc_Play_Sound(sounds[4],lengths[4]);
}

video_mem[12802+level.main_char_gr*45]=0;
video_mem[12803+level.main_char_gr*45]=0;
video_mem[13122+level.main_char_gr*45]=0;

video_mem[12845+level.main_char_gr*45]=0;
video_mem[12846+level.main_char_gr*45]=0;
video_mem[13166+level.main_char_gr*45]=0;

video_mem[27202-320+level.main_char_gr*45]=0;
video_mem[27203-320+level.main_char_gr*45]=0;
video_mem[26882-320+level.main_char_gr*45]=0;

video_mem[27246-320+level.main_char_gr*45]=0;
video_mem[27245-320+level.main_char_gr*45]=0;
video_mem[26926-320+level.main_char_gr*45]=0;

if(inp=='4')
    level.main_char_gr--;
if(inp=='6')
    level.main_char_gr++;
 if(level.main_char_gr>=7)
    level.main_char_gr=0;
if(level.main_char_gr<0)
    level.main_char_gr=6;
}

Voc_Unload_Sound(sounds[4]);

//level.main_char_grf=char_num;

fp=fopen("numbers.grf","rb");
for(num=0;num<1440;num=num+1)
    {
    numbers[num]=getc(fp);
    }
printf("\r Read.    ");
fclose(fp);
printf("\r Closed.   ");

printf("\n Load the set of numbers for the timer.");

for(num=0;num<768;num++)
    curr_pal[num]=0;

printf("\n Zero out the palette array.");

fp=fopen("mug.grf","rb");
if(!(fp))
printf("\n The mug shots have been missplaced!\n");
fread(&mug,1,12696,fp);
fclose(fp);

printf("\n Load the mug shots.");


printf("\n Load the explosion data.");

setvideomode(3);
printf("\n\n  Choose the battle sight.\n");
printf("\n      1.  The Dead City.");
printf("\n      2.  The Great Maze of Pipes.");
printf("\n      3.  The War Room.");
printf("\n      4.  And of cause ... THE HALL OF DOOM!\n");

inp=getch();

if(inp=='1')
    level.level_graph=fopen("deadcity.grf","rb");
if(inp=='2')
    level.level_graph=fopen("pipes.grf","rb");
if(inp=='3')
    level.level_graph=fopen("warroom.grf","rb");
if(inp=='4')
    level.level_graph=fopen("doom.grf","rb");
if(inp=='7')
    level.level_graph=fopen("instruct.grf","rb");

level.char_graphs[0]=fopen("acrobat.grf","rb");
level.char_graphs[1]=fopen("imp.grf","rb");
level.char_graphs[2]=fopen("normal.grf","rb");
level.char_graphs[3]=fopen("normal.grf","rb");
level.char_graphs[4]=fopen("imp.grf","rb");


fp=fopen("expl.you","rb");
for(num=0;num<=level.main_char_gr;num++)
    for(num=0;num<500;num++)
	green_ball.expl_graph[num]=getc(fp);
fclose(fp);

level.proj_graph[0]=fopen("fireball.grf","rb");
level.proj_graph[1]=fopen("fireball.grf","rb");
level.proj_graph[2]=fopen("missle.grf","rb");
level.proj_graph[3]=fopen("missle.grf","rb");
level.proj_graph[4]=fopen("fireball.grf","rb");

//if(inp=='1')
    level.main_proj_gr=fopen("missle.grf","rb");
//if(inp=='2')
//    level.main_proj_gr=fopen("fireball.grf","rb");
//if(inp=='3')
//    level.main_proj_gr=fopen("fireball.grf","rb");
//if(inp=='4')
//    level.main_proj_gr=fopen("bulmis.grf","rb");

level.expl_graph[0]=fopen("ffsshh.grf","rb");
level.expl_graph[1]=fopen("ffsshh.grf","rb");
level.expl_graph[2]=fopen("boom.grf","rb");
level.expl_graph[3]=fopen("boom.grf","rb");
level.expl_graph[4]=fopen("ffsshh.grf","rb");

//level.main_expl_gr=fopen("boom.grf","rb");

//level.level_title=fopen("pipemaze.grf","rb");

level.start_seconds=0;
level.start_minutes=0;
level.start_hours=5;

lemming.dir=0;
lemming.frame=0;
lemming.action=3;
lemming.jump=0;
lemming.fall=3;
lemming.x=100;
lemming.y=40;
lemming.dead_alive=1;
lemming.back_up=1;

inp=level.main_char_gr;

if(inp==0)
{
green_ball.missle_speed[0]=0;
green_ball.missle_speed[1]=0;
green_ball.expl_frame=5;
green_ball.missle_start_sp[0]=10;
green_ball.missle_start_sp[1]=-5;
green_ball.missle_fall=1;
green_ball.missle_range=5;
}
if(inp==1)
{
green_ball.missle_speed[0]=0;
green_ball.missle_speed[1]=0;
green_ball.expl_frame=5;
green_ball.missle_start_sp[0]=30;
green_ball.missle_start_sp[1]=0;
green_ball.missle_fall=0;
green_ball.missle_range=15;
}
if(inp==2)
{
green_ball.missle_speed[0]=0;
green_ball.missle_speed[1]=0;
green_ball.expl_frame=5;
green_ball.missle_start_sp[0]=15;
green_ball.missle_start_sp[1]=-3;
green_ball.missle_fall=1;
green_ball.missle_range=10;
}

if(inp==3)
{
green_ball.missle_speed[0]=0;
green_ball.missle_speed[1]=0;
green_ball.expl_frame=5;
green_ball.missle_start_sp[0]=15;
green_ball.missle_start_sp[1]=0;
green_ball.missle_fall=0;
green_ball.missle_range=10;
}
if(inp==4)
{
green_ball.missle_speed[0]=0;
green_ball.missle_speed[1]=0;
green_ball.expl_frame=5;
green_ball.missle_start_sp[0]=3;
green_ball.missle_start_sp[1]=-5;
green_ball.missle_fall=1;
green_ball.missle_range=15;
}
if(inp==5)
{
green_ball.missle_speed[0]=0;
green_ball.missle_speed[1]=0;
green_ball.expl_frame=5;
green_ball.missle_start_sp[0]=3;
green_ball.missle_start_sp[1]=-1;
green_ball.missle_fall=0;
green_ball.missle_range=15;
}
if(inp==6)
{
green_ball.missle_speed[0]=0;
green_ball.missle_speed[1]=0;
green_ball.expl_frame=5;
green_ball.missle_start_sp[0]=1;
green_ball.missle_start_sp[1]=1;
green_ball.missle_fall=0;
green_ball.missle_range=30;
}

bad_lem[0].dir	      = 0;
bad_lem[0].frame      = 0;
bad_lem[0].action     = 3;
bad_lem[0].jump       = 0;
bad_lem[0].fall       = 5;
bad_lem[0].x	      = 500;
bad_lem[0].y	      = 30;
bad_lem[0].back_up = 1;
bad_lem[0].dead_alive=1;

bad_lem[1].dir	      = 1;
bad_lem[1].frame      = 2;
bad_lem[1].action     = 2;
bad_lem[1].jump       = 0;
bad_lem[1].fall       = 1;
bad_lem[1].x	      = 540;
bad_lem[1].y	      = 10;
bad_lem[1].back_up = 1;
bad_lem[1].dead_alive=1;

bad_lem[2].dir	      = 0;
bad_lem[2].frame      = 3;
bad_lem[2].action     = 1;
bad_lem[2].jump       = 0;
bad_lem[2].fall       = 0;
bad_lem[2].x	      = 510;
bad_lem[2].y	      = 70;
bad_lem[2].back_up = 1;
bad_lem[2].dead_alive=1;

bad_lem[3].dir	      = 1;
bad_lem[3].frame      = 0;
bad_lem[3].action     = 0;
bad_lem[3].jump       = 0;
bad_lem[3].fall       = 7;
bad_lem[3].x	      = 515;
bad_lem[3].y	      = 15;
bad_lem[3].back_up = 1;
bad_lem[3].dead_alive=1;

bad_lem[4].dir	      = 1;
bad_lem[4].frame      = 2;
bad_lem[4].action     = 0;
bad_lem[4].jump       = 0;
bad_lem[4].fall       = 3;
bad_lem[4].x	      = 550;
bad_lem[4].y	      = 20;
bad_lem[4].back_up = 1;
bad_lem[4].dead_alive=1;

blue_ball[0].missle_speed[0]=0;
blue_ball[0].missle_speed[1]=0;
blue_ball[0].expl_frame=5;
blue_ball[0].missle_start_sp[0]=5;
blue_ball[0].missle_start_sp[1]=-5;
blue_ball[0].missle_fall=1;
blue_ball[0].missle_range=10;

blue_ball[1].missle_speed[0]=0;
blue_ball[1].missle_speed[1]=0;
blue_ball[1].expl_frame=5;
blue_ball[1].missle_start_sp[0]=10;
blue_ball[1].missle_start_sp[1]=-5;
blue_ball[1].missle_fall=1;
blue_ball[1].missle_range=5;

blue_ball[2].missle_speed[0]=0;
blue_ball[2].missle_speed[1]=0;
blue_ball[2].expl_frame=5;
blue_ball[2].missle_start_sp[0]=15;
blue_ball[2].missle_start_sp[1]=-3;
blue_ball[2].missle_fall=1;
blue_ball[2].missle_range=10;

blue_ball[3].missle_speed[0]=0;
blue_ball[3].missle_speed[1]=0;
blue_ball[3].expl_frame=5;
blue_ball[3].missle_start_sp[0]=8;
blue_ball[3].missle_start_sp[1]=-3;
blue_ball[3].missle_fall=1;
blue_ball[3].missle_range=5;

blue_ball[4].missle_speed[0]=0;
blue_ball[4].missle_speed[1]=0;
blue_ball[4].expl_frame=5;
blue_ball[4].missle_start_sp[0]=10;
blue_ball[4].missle_start_sp[1]=-4;
blue_ball[4].missle_fall=1;
blue_ball[4].missle_range=10;

brains[0].move_or_not  = 40;
brains[0].to_or_from   = 50;
brains[0].shoot_or_not = 20;
brains[0].jump_or_not  = 90;
brains[0].dodge_or_not = 35;

brains[1].move_or_not  = 80;
brains[1].to_or_from   = 95;
brains[1].shoot_or_not = 95;
brains[1].jump_or_not  = 100;
brains[1].dodge_or_not = 100;

brains[2].move_or_not  = 60;
brains[2].to_or_from   = 50;
brains[2].shoot_or_not = 40;
brains[2].jump_or_not  = 25;
brains[2].dodge_or_not = 65;

brains[3].move_or_not  = 20;
brains[3].to_or_from   = 10;
brains[3].shoot_or_not = 90;
brains[3].jump_or_not  = 75;
brains[3].dodge_or_not = 90;

brains[4].move_or_not  = 30;
brains[4].to_or_from   = 75;
brains[4].shoot_or_not = 95;
brains[4].jump_or_not  = 99;
brains[4].dodge_or_not = 99;

//play_level(level,(sprite_typ *)&lemming,(missle_typ *)&green_ball,(sprite_typ *)&bad_lem,(missle_typ *)&blue_ball,(brain_typ *)&brains);
//play_level(level,
//	   lemming,
//	   green_ball,
//	   bad_lem[0],
//	   bad_lem[1],
//	   bad_lem[2],
//	   bad_lem[3],
//	   bad_lem[4],
//	   blue_ball[0],
//	   blue_ball[1],
//	   blue_ball[2],
//	   blue_ball[3],
//	   blue_ball[4],
//	   brains[0],

//	   );

map_x=160;
map_y=100;

setvideomode(19);
view_picture("hellstg.grf",3,0);
x=map_x;
y=map_y;
map_x=rand()%319;
map_y=rand()%199;

while(x!=map_x || y!=map_y)
    {
    if(rand()%100<5)
    {
    if(x<map_x)
	x++;
    if(x>map_x)
	x--;
    if(y<map_y)
	y++;
    if(y>map_y)
	y--;
    }
    video_mem[(x+rand()%3)+(y+rand()%3)*320]=5;
    }

for(num=0;num<5;num++)
    bad_lem[num].dead_alive=1;
play_level();
fcloseall();
for(num=0;num<256;num++)
    set_color(num,pal[num*3],pal[num*3+1],pal[num*3+2]);
view_picture("gameover.grf",3,4);
view_picture("thanks.grf",2,6);

fade_out(1);

Voc_Set_Speaker(0);
Voc_Unload_Sound(sounds[2]);
Voc_Unload_Sound(sounds[3]);
Voc_Unload_Sound(sounds[1]);
Voc_Unload_Sound(sounds[0]);
Voc_Terminate_Driver();



setvideomode(3);
	   //klk
}
