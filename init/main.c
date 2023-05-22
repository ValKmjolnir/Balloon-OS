// balloon system init/main.c
// copyright @ValKmjolnir
// 2021

#ifndef __MAIN_C__
#define __MAIN_C__

#define NULL 0
#include "../kernel/cstring.h"
#include "../kernel/printk.h"
#include "../kernel/input.h"

long system_stack[2048];
struct {
    long *esp; // esp &system_stack[2048]
    short ss;  // ss  0x10
} stack_start = {&system_stack[2048],0x10};

extern uint64_t idt[256];

// init vga 16 color memory
void init_vga_memory(int color) {
    // 320 pixels in a line 0x140
    for(unsigned int i=0xa0000;i<0xb0000;++i) {
        *(char*)i=color;
    }
    return;
}

int help();

int version() {
    printk("Balloon OS v0.0.5\n");
    printk("___  ____          __   __  _  _\n");
    printk("|__] |__| |   |   |  | |  | |\\ |\n");
    printk("|__] |  | |__ |__ |__| |__| | \\|\n");
    return 0;
}

int reboot() {
    // keyboard controller error reboot
    asm(
        "movb $0xfe,%al\n"
        "outb %al,$0x64\n"
    );
    // write 0x0e to 0xcf9 reset but not usefull in virtualbox
    // 0x02 soft reset
    // 0x06 hard reset
    // 0x0e full reset
    asm(
        "xor %dx,%dx\n"
        "xor %al,%al\n"
        "mov $0xcf9,%dx\n"
        "in %dx,%al\n"
        "mov $0xcf9,%dx\n"
        "orb $0x0e,%al\n"
        "out %al,%dx"
    );
    return 0;
}

int shutdown() {
    asm(
        "mov $0x5301,%ax\n"
        "xor %bx,%bx\n"
        "int $0x15"
    );
    return 0;
}

struct {
    char* cmd_name;
    int (*func_ptr)();
} cmd_info[]= {
    {"help", help},
    {"version", version},
    {"reset", reboot},
    {"shutdown", shutdown},
    {"clear", clean},
    {NULL, NULL}
};

int help() {
    for(int i=0;cmd_info[i].cmd_name;++i) {
        printk("%s\n",cmd_info[i].cmd_name);
    }
}

void execcmd(const char* buf) {
    for(int i=0;cmd_info[i].cmd_name;++i) {
        if(!strcmp(cmd_info[i].cmd_name,buf)) {
            cmd_info[i].func_ptr();
            return;
        }
    }
    printk("%s: command not found\n", buf);
    return;
}

int main() {
    char buf[256];
    clean();
    printk("Balloon OS v0.0.5\n");
    printk("Copyright @ValKmjolnir 2020-2023\n");
    printk("https://github.com/ValKmjolnir/Balloon-OS\n");
    printk("idt[%d] %p %p\n",80,((uint32_t)(idt[80]>>32))&0xffffffff,(uint32_t)(idt[80]&0xffffffff));

    while(1) {
        printk("$> ");
        getline(255,buf);
        execcmd(buf);
    }
    return 0;
}
#endif
