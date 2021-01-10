// balloon system kernel/printk.c
// copyright @ValKmjolnir
// 2020

#include "font.h"
#include "vsprintf.c"

#define VGA_BLACK 0
#define VGA_PINK 63
// set pixel at 320x200 vga ram
#define setpixel(base,x,y,color) (*(char*)(base+(x)+((y)<<8)+((y)<<6))=color)

unsigned short line=0;
unsigned short column=0;
char printk_buffer[512];
char screen[32][10];
void putk(char* base,char c)
{
	++base;
	for(unsigned short i=0;i<16;++i)
	{
		unsigned char tmp=font_table[c][i];
		for(unsigned short j=0;j<8;++j)
			setpixel(base+1,j,i,((1<<(7-j))&tmp)?VGA_BLACK:VGA_PINK);
	}
	return;
}
void flush()
{
	char* base=0xa0000;
	const unsigned int lne_jmp=320*18;
	for(unsigned short lne=0;lne<10;++lne)
	{
		for(unsigned short col=0;col<32;++col)
		{
			putk(base,screen[col][lne]);
			base+=10;
		}
		base+=lne_jmp;
	}
	return;
}
void shift()
{
	for(unsigned short lne=1;lne<10;++lne)
		for(unsigned short col=0;col<32;++col)
			screen[col][lne-1]=screen[col][lne];
	return;
}
int printk(const char* fmt,...)
{
	va_list arg;
	va_start(arg,fmt);
	int cnt=vsprintf(printk_buffer,fmt,arg);
	for(char* tmp=printk_buffer;*tmp;++tmp)
	{
		if(*tmp=='\n')      ++line;
		else if(*tmp=='\r') column=0;
		else screen[column++][line]=*tmp;
		if(column>=32)
		{
			column=0;
			++line;
		}
		if(line>=10)
		{
			line=10;
			shift();
		}
	}
	va_end(arg);
	flush();
	return cnt;
}
