#ifndef __PRINTK_H__
#define __PRINTK_H__

#include <stdint.h>
#include <stdarg.h>
#define VGA_BEGIN 0xb8000
#define SCREEN_WID 80
#define SCREEN_HGT 25

char printk_buff[512];
uint8_t screen_x=0;
uint8_t screen_y=0;

void balloon_putc(uint8_t x,uint8_t y,char c) {
    uint16_t* monitor=(uint16_t*)VGA_BEGIN;
    uint32_t idx=x+y*SCREEN_WID;
    monitor[idx]=(monitor[idx]&0xff00)|c;
    return;
}

int clean() {
    screen_x=screen_y=0;
    for(uint8_t y=0;y<SCREEN_HGT;++y) {
        for(uint8_t x=0;x<SCREEN_WID;++x) {
            balloon_putc(x,y,'\0');
        }
    }
    return 0;
}

void moveup() {
    uint16_t* monitor=(uint16_t*)VGA_BEGIN;
    for(uint8_t y=1;y<SCREEN_HGT;++y) {
        for(uint8_t x=0;x<SCREEN_WID;++x) {
            uint32_t idx=x+y*SCREEN_WID;
            uint32_t prev=x+(y-1)*SCREEN_WID;
            monitor[prev]=monitor[idx];
        }
    }
    for(uint8_t x=0;x<SCREEN_WID;++x) {
        uint32_t idx=x+(SCREEN_HGT-1)*SCREEN_WID;
        monitor[idx]=(monitor[idx]&0xff00)|'\0';
    }
    return;
}

char* vprintk_hex_num(char* str, long num) {
    char tmp[8];
    for(int i=0;i<8;++i) {
        tmp[i]='0';
    }
    for(int i=0;i<8;++i) {
        int hex_bit=(num&15);
        tmp[7-i]=(hex_bit>9? 'a'+hex_bit-10:'0'+hex_bit);
        num>>=4;
    }
    for(int ptr=0;ptr<8;++ptr)
        *str++=tmp[ptr];
    return str;
}

void vprintk(const char* fmt,va_list ap) {
    char* tmp=printk_buff;
    for(int i=0;fmt[i] && tmp-printk_buff<511;++i) {
        if(fmt[i]=='%' && fmt[i+1]) {
            ++i;
            switch(fmt[i]) {
                case 'c':*tmp++=va_arg(ap,int);break;
                case 'd': {
                        int num=va_arg(ap,int);
                        if(!num) {
                            *tmp++='0';
                        }
                        if(num<0) {
                            *tmp++='-';
                            num=-num;
                        }
                        int width=1;
                        while(num/width) {
                            width*=10;
                        }
                        width/=10;
                        while(width) {
                            *tmp++=(char)('0'+num/width);
                            num%=width;
                            width/=10;
                        }
                    }
                    break;
                case 's': {
                        char* s=va_arg(ap,char*);
                        for(int i=0;s[i];++i) {
                            *tmp++=s[i];
                        }
                    }
                    break;
                case 'p':
                    *tmp++='0';
                    *tmp++='x';
                    tmp=vprintk_hex_num(tmp,(unsigned long)va_arg(ap,void *));
                    break;
                case '%':*tmp++='%';break;
                default:--i;*tmp++='%';break;
            }
            continue;
        }
        *tmp++=fmt[i];
    }
    *tmp='\0';
    return;
}

void printk(const char* fmt,...) {
    va_list ap;
    va_start(ap,fmt);
    vprintk(fmt,ap);
    va_end(ap);
    for(char* tmp=printk_buff;*tmp;++tmp) {
        if(*tmp=='\n') {
            screen_x=0;
            screen_y++;
            if(screen_y>=SCREEN_HGT) {
                screen_y=SCREEN_HGT-1;
                moveup();
            }
        } else if(*tmp=='\r') {
            screen_x=0;
        } else if(*tmp=='\b') {
            screen_x=screen_x>0?screen_x-1:screen_x;
            balloon_putc(screen_x,screen_y,' ');
        } else {
            if(screen_x>=SCREEN_WID) {
                screen_x=0;
                screen_y++;
            }
            if(screen_y>=SCREEN_HGT) {
                screen_x=0;
                screen_y=SCREEN_HGT-1;
                moveup();
            }
            balloon_putc(screen_x,screen_y,*tmp);
            ++screen_x;
        }
    }
    return;
}

#endif