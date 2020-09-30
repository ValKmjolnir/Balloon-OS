// balloon system kernel/vsprintf.c
// copyright @ValKmjolnir
// 2020

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

#define FLAG_LEFT  1
#define FLAG_PLUS  2
#define FLAG_SPACE 4
#define FLAG_HASH  8
#define FLAG_ZERO  16
#define IS_FLAGS(c) (c=='-' || c=='+' || c==' ' || c=='#' || c=='0')
#define IS_DIGIT(c) ('0'<=c && c<='9')

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
			*buf++=*fmt;
			continue;
		}
		++fmt;
		// flags
        flags=0;
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
        width=-1;
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
        precision=-1;
		if(*fmt=='.')
		{
			++fmt;
			if(*fmt=='*')
				precision=va_arg(args,int);
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
