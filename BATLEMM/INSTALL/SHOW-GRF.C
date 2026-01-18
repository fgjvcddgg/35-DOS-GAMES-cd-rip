#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <dos.h>
#include "palette.fct"

#define TRUE  1
#define FALSE 0
#define OK    1
#define ERR   0
#define PALETTE_MASK	 0x3c6
#define PALETTE_REGISTER 0x3c8
#define PALETTE_DATA	 0x3c9

char far *video_mem=(char far *)0xA0000000L;
char curr_pal[768];

void setvideomode(unsigned int mode)
{
union REGS regs;

regs.x.ax=mode;
int86(0x10,&regs,&regs);
}

void set_color(int index, char red, char green, char blue)
{
outp(PALETTE_MASK,0xff);
outp(PALETTE_REGISTER,index);
outp(PALETTE_DATA,red);
outp(PALETTE_DATA,green);
outp(PALETTE_DATA,blue);
}

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
for(x=0;x<90;x++)
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
    video_mem[x+y*320]=double_buffer[x+y*320];
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
    how_fast[num]=rand()%10+5;
//    }
    }
for(num=0;num<50;num++)
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
    video_mem[num2+x*320]=double_buffer[num2+(200-how_high[num2]+x)*320];
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

//printf("\n Press any key to continue.\n");
inp=getch();
}

void main(argc,argv)
int argc;
char *argv[];
{
   char pcxname[13];

   strncpy(pcxname,argv[1],13);
   if (!check_suffix(pcxname))
   {
      printf("unable to open grf file: %s",argv[1]);
      exit(1);
   }

   setvideomode(19);

   fade_in(5);

   view_picture(pcxname,3,7);
   setvideomode(3);

   exit(0);
}

check_suffix(char *fname)
{
   char *strptr;
   int index;
   int error;

   error = FALSE;
   strptr = strchr(fname,'.');

   /* period in string */
   if (strptr > 0)
   {
      index = (int)(strptr - fname);
      if (index > 8)
      error = TRUE;
   }

   /* no period in string */
   else
   {
      index = strlen(fname);
      if (index > 8)
        error = TRUE;
      else
      {
        fname[index] = '.';
	strcat(fname,"grf");
      }
   }
   if (error) return(ERR);
   return(OK);
}

