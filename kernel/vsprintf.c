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
#define FLAG_SMALL 32
#define FLAG_SIGN  64
#define IS_FLAGS(c) ((c)=='-' || (c)=='+' || (c)==' ' || (c)=='#' || (c)=='0')
#define IS_DIGIT(c) ('0'<=(c) && (c)<='9')

enum vsprintf_length
{
	len_none,
	len_hh,len_h,
	len_l,len_ll,
	len_j,
	len_z,
	len_t,
	len_L
};

int vsprintf(char* buf,const char* fmt,va_list args)
{
	int flags,width,precision,qualifier;
	char* str;
	// reserved for va_args
	char* va_str;
	int str_len;
	char* tmp;
	int i;
	for(str=buf;*fmt;++fmt)
	{
		if(*fmt!='%')
		{
			*str++=*fmt;
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
			width=va_arg(args,int);
			++fmt;
			if(width<0)
			{
				width=-width;
				flags|=FLAG_LEFT;
			}
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
			{
				precision=va_arg(args,int);
				++fmt;
			}
			else
				while(IS_DIGIT(*fmt))
				{
					precision=precision*10+(*fmt-'0');
					++fmt;
				}
			if(precision<0)
				precision=0;
		}
		// qualifier
		// this is not in use temporarily...
		qualifier=len_none;
		switch(*fmt)
		{
			case 'h':
				++fmt;
				qualifier=len_h;
				if(*fmt=='h')
				{
					qualifier=len_hh;
					++fmt;
				}
				break;
			case 'l':
				++fmt;
				qualifier=len_l;
				if(*fmt=='l')
				{
					qualifier=len_ll;
					++fmt;
				}
				break;
			case 'j':qualifier=len_j;++fmt;break;
			case 'z':qualifier=len_z;++fmt;break;
			case 't':qualifier=len_t;++fmt;break;
			case 'L':qualifier=len_L;++fmt;break;
		}
		// data type
		switch(*fmt)
		{
			case 'c':
				if(!(flags&FLAG_LEFT))
					while(width>0)
					{
						*str++=' ';
						--width;
					}
				*str++=(unsigned char)va_arg(args,int);
				while(width>0)
				{
					*str++=' ';
					--width;
				}
				break;
			case 's':
				va_str=va_arg(args,char*);
				str_len=0;
				for(tmp=va_str;*tmp;++tmp)
					++str_len;
				if(precision<0)
					precision=str_len;
				else if(str_len>precision)
					str_len=precision;
				if(!(flags&FLAG_LEFT))
					while(width>str_len)
					{
						*str++=' ';
						--width;
					}
				for(i=0;i<str_len;++i)
					*str++=va_str[i];
				while(width>str_len)
				{
					*str++=' ';
					--width;
				}
				break;
			case 'o':break;
			case 'p':break;
			case 'x':flags|=FLAG_SMALL;
			case 'X':break;
			case 'd':break;
			case 'i':flags|=FLAG_SIGN;
			case 'u':break;
			case 'n':break;
			default:
				if(*fmt!='%')
					*str++='%';
				if(*fmt)
					*str++=*fmt;
				else
					--fmt;
				break;
		}
	}
	*str='\0';
	return str-buf;
}
