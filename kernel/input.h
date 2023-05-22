#ifndef __GETCHAR_H__
#define __GETCHAR_H__

#define esc       0x1b
#define backspace '\b'
#define tab       '\t'
#define enter     '\n'
#define delete    0x7f
#define lctrl     ' '
#define rctrl     ' '
#define lshift    ' '
#define rshift    ' '
#define lalt      ' '
#define ralt      ' '
#define capslock  ' '

#define lshift_make   0x2a
#define rshift_make   0x36
#define lalt_make     0x38
#define ralt_make     0xe038
#define ralt_break    0xe0b8
#define lctrl_make    0x1d
#define rctrl_make    0xe01d
#define rctrl_break   0xe09d
#define capslock_make 0x3a

char keyboard_map[256][2] = {
    {'\0','\0'},// 0x00
    {esc,esc},// 0x01
    {'1','!'},//0x02
    {'2','@'},//0x03
    {'3','#'},//0x04
    {'4','$'},//0x05
    {'5','%'},//0x06
    {'6','^'},//0x07
    {'7','&'},//0x08
    {'8','*'},//0x09
    {'9','('},//0x0a
    {'0',')'},//0x0b
    {'-','_'},//0x0c
    {'=','+'},//0x0d
    {backspace,backspace},//0x0e
    {tab,tab},//0x0f
    {'q','Q'},//0x10
    {'w','W'},//0x11
    {'e','E'},//0x12
    {'r','R'},//0x13
    {'t','T'},//0x14
    {'y','Y'},//0x15
    {'u','U'},//0x16
    {'i','I'},//0x17
    {'o','O'},//0x18
    {'p','P'},//0x19
    {'[','{'},//0x1a
    {']','}'},//0x1b
    {enter,enter},//0x1c
    {lctrl,lctrl},//0x1d
    {'a','A'},//0x1e
    {'s','S'},//0x1f
    {'d','D'},//0x20
    {'f','F'},//0x21
    {'g','G'},//0x22
    {'h','H'},//0x23
    {'j','J'},//0x24
    {'k','K'},//0x25
    {'l','L'},//0x26
    {';',':'},//0x27
    {'\'','\"'},//0x28
    {'`','~'},//0x29
    {lshift,lshift},//0x2a
    {'\\','|'},//0x2b
    {'z','Z'},//0x2c
    {'x','X'},//0x2d
    {'c','C'},//0x2e
    {'v','V'},//0x2f
    {'b','B'},//0x30
    {'n','N'},//0x31
    {'m','M'},//0x32
    {',','<'},//0x33
    {'.','>'},//0x34
    {'/','?'},//0x35
    {rshift,rshift},//0x36
    {'*','*'},//0x37
    {lalt,lalt},//0x38
    {' ',' '},//0x39
    {capslock,capslock}//0x3a
};

uint8_t inb(int port) {
    uint8_t data;
    __asm__ volatile(
        "inb %w1,%0"
        :"=a" (data)
        :"d" (port)
    );
    return data;
}

int getchar() {
    uint8_t stat, data;

    stat = inb(0x64);// keyboard controller status port
    if ((stat & 0x01) == 0) {
        return -1;
    }

    data = inb(0x60);// keyboard I/O port
    if (data<0x3b) {
        return keyboard_map[data][0];
    }
    return data;
}

uint32_t getline(uint32_t len,char* buf) {
    uint32_t cnt=0;
    while(cnt<len) {
        char c=getchar();
        if(c<=0) {
            continue;
        }
        if(c=='\n' || c=='\r') {
            printk("%c",c);
            break;
        }
        if(c=='\b') {
            if(cnt) {
                --cnt;
                printk("\b");
            }
            continue;
        }
        printk("%c",c);
        buf[cnt++]=c;
    }
    buf[cnt]='\0';
    return cnt;
}
#endif