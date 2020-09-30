// balloon system kernel/printk.c
// copyright @ValKmjolnir
// 2020

#include "font.h"
#include "vsprintf.c"

// black 0
#define VGA_BLACK 0
// pink 63
#define VGA_PINK 63
// base address of vga 13h 320x200 video ram
char* pixel_pointer=0xa0000+1;
// line boundary of pixel_pointer
char* pixel_line_boundary=0xa0000+320;
// set pixel at 320x200 vga ram
#define setpixel(base,x,y,color) (*(char*)(base+(x)+(y)*320)=color)

void _putchar(char c)
{
	if(c=='\n' || c=='\r')
	{
		switch(c)
		{
			case '\n':pixel_line_boundary+=320*16;pixel_pointer+=320*16;break;
			case '\r':pixel_pointer=pixel_line_boundary-320+1;break;
		}
		return;
	}
    for(int i=0;i<16;++i)
    {
    	unsigned char tmp=font_table[c][i];
    	for(int j=7;j>=0;--j)
		{
			if((1<<j)&tmp)
    			setpixel(pixel_pointer,7-j,i,VGA_BLACK);
			else
				setpixel(pixel_pointer,7-j,i,VGA_PINK);
		}
	}
	pixel_pointer+=8;
	if(pixel_pointer>=pixel_line_boundary)
	{
		pixel_line_boundary+=320*16;
		pixel_pointer=pixel_line_boundary-320+1;
	}
    return;
}

char printk_buffer[1024];
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
