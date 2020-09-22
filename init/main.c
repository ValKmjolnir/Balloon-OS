// balloon system init/main.c
// copyright @ValKmjolnir
// 2020
#include <stdarg.h>
#include "prt_char.h"

#define PAGE_SIZE 4096
long kernel_stack[PAGE_SIZE>>2];
struct {
    long *a;
    short b;
}stack_start={&kernel_stack[PAGE_SIZE>>2],0x10};
// esp is set &kernel_stack[PAGE_SIZE>>2],ss is set 0x10->0b00010 0 00 (index 2 in gdt(0) 0 level[highest])

void printk(char* buf,...)
{
    return;
}

// init vga 16 color memory
void init_vga_memory(int color)
{
    // 320 pixels in a line 0x140
    int i;
    for(i=0xa0000;i<0xb0000;++i)
        *(char*)i=color;
    return;
}

// head.s jumps here
void main()
{
    init_vga_memory(63);
    print_char('>',0xa0140);
    print_char('h',0xa0140+4);
    print_char('e',0xa0140+8);
    print_char('l',0xa0140+12);
    print_char('l',0xa0140+16);
    print_char('o',0xa0140+20);
    while(1)
    {
        __asm__("hlt");
    }
    return;
}
