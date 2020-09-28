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


#define VGA_BLACK 0
#define VGA_PINK 63
// base address of vga 13h 320x200 video ram
char* pixel_pointer=0xa0000+1;
// line boundary of pixel_pointer
char* pixel_line_boundary=0xa0000+320;

#define setpixel(base,x,y,color) (*(char*)(base+(x)+(y)*320)=color)

void sys_putchar(char c)
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
	for(;*fmt;++fmt)
	{
		if(*fmt!='%')
		{
			*buf=*fmt;
			++buf;
			continue;
		}
		++fmt;
		// unfinished
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
		sys_putchar(*tmp);
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
