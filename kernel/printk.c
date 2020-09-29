#include "font.h"
// defined for dynamic arguments
typedef char* va_list;
// sizeof(n) =1/2/4/8 _INTSIZEOF(n)=4/4/4/8
#define _INTSIZEOF(n) ((sizeof(n)+sizeof(int)-1)&~(sizeof(int)-1))
// get dynamic arguments' base address
#define va_start(ap,v) (ap=(va_list)&v+_INTSIZEOF(v))
// add ap pointer to the next place but get the value before adding
#define va_arg(ap,t) (*(t*)((ap+=_INTSIZEOF(t))-_INTSIZEOF(t)))
// avoid null pointer
#define va_end(ap) (ap=(va_list)0)

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

#define FLAG_LEFT  1
#define FLAG_PLUS  2
#define FLAG_SPACE 4
#define FLAG_HASH  8
#define FLAG_ZERO  16
#define IS_FLAGS(c) (c=='-' || c=='+' || c==' ' || c=='#' || c=='0')
#define IS_DIGIT(c) ('0'<=c && c<='9')


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

int vsprintf(char* buf,const char* fmt,va_list args)
{
	int cnt=0;
	int flags=0;
	int width=-1;
	int precision=-1;
	for(;*fmt;++fmt)
	{
		if(*fmt!='%')
		{
			*buf=*fmt;
			++buf;
			continue;
		}
		++fmt;
		// flags
		while(IS_FLAGS(*fmt))
		{
			switch(*fmt)
			{
				case '-':flags|=FLAG_LEFT;break;
				case '+':flags|=FLAG_PLUS;break;
				case ' ':flags|=FLAG_SPACE;break;
				case '#':flags|=FLAG_HASH;break;
				case '0':flags|=FLAG_ZERO;break;
			}
			++fmt;
		}
		// width
		if(*fmt=='*')
		{
			width=-1;
			++fmt;
		}
		else
			while(IS_DIGIT(*fmt))
			{
				width=width*10+(*fmt-'0');
				++fmt;
			}
		// precision
		if(*fmt=='.')
		{
			++fmt;
			if(*fmt=='*')
				precision=-1;
			else
				while(IS_DIGIT(*fmt))
				{
					precision=precision*10+(*fmt-'0');
					++fmt;
				}
			++fmt;
		}
		// length unfinished
	}
	return cnt;
}

int printk(const char* fmt,...)
{
    va_list arg;
    char buffer[256];

    va_start(arg,fmt);
    int cnt=vsprintf(buffer,fmt,arg);
	for(char* tmp=buffer;*tmp;++tmp)
		_putchar(*tmp);
    va_end(arg);
    return cnt;
}

// int main()
// {
//     printf("printf test\n\r") ;	
// 	printf("test char    =%c,%c\n\r", 'A','a') ;	
// 	printf("test decimal =%d\n\r",    123456) ;
// 	printf("test decimal =%d\n\r",    -123456) ;	
// 	printf("test hex     =0x%x\n\r",  0x55aa55aa) ;	
// 	printf("test string  =%s\n\r",    "hello") ;	
// 	printf("num=%08d\n\r",   12345);
// 	printf("num=%8d\n\r",    12345);
// 	printf("num=0x%08x\n\r", 0x12345);
// 	printf("num=0x%8x\n\r",  0x12345);
// 	printf("num=0x%02x\n\r", 0x1);
// 	printf("num=0x%2x\n\r",  0x1);
// 	printf("num=%05d\n\r",   0x1);
// 	printf("num=%5d\n\r",    0x1);
//     return 0;
// }
