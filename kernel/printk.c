// balloon system kernel/printk.c
// copyright @ValKmjolnir
// 2020

#include "font.h"
#include "vsprintf.c"

#define VGA_BLACK 0
#define VGA_PINK 63
// base address of vga 13h 320x200 video ram
char* pixel_pointer=0xa0000+1;
// line boundary of pixel_pointer
char* pixel_line_boundary=0xa0000+320;
// set pixel at 320x200 vga ram
#define setpixel(base,x,y,color) (*(char*)(base+(x)+((y)<<8)+((y)<<6))=color)

void _putchar(char c)
{
	unsigned int jmp_line=320<<4;
	if(c=='\n' || c=='\r')
	{
		switch(c)
		{
			case '\n':
				pixel_line_boundary+=jmp_line;
				pixel_pointer+=jmp_line;
				break;
			case '\r':
				pixel_pointer=pixel_line_boundary-320+1;
				break;
		}
		return;
	}
	for(int i=0;i<16;++i)
	{
		unsigned char tmp=font_table[c][i];
		for(int j=7;j>=0;--j)
			setpixel(pixel_pointer,7-j,i,((1<<j)&tmp)?VGA_BLACK:VGA_PINK);
	}
	pixel_pointer+=8;
	if(pixel_pointer>=pixel_line_boundary)
	{
		pixel_line_boundary+=jmp_line;
		pixel_pointer=pixel_line_boundary-320+1;
	}
    return;
}

char printk_buffer[2048];
int printk(const char* fmt,...)
{
	va_list arg;
	va_start(arg,fmt);
	int cnt=vsprintf(printk_buffer,fmt,arg);
	for(char* tmp=printk_buffer;*tmp;++tmp)
		_putchar(*tmp);
	va_end(arg);
	return cnt;
}
