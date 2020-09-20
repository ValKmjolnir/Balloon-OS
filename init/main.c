#define PAGE_SIZE 4096
long user_stack[PAGE_SIZE>>2];
struct {
    long *a;
    short b;
}stack_start={&user_stack[PAGE_SIZE>>2],0x10};
void printk(char* buf)
{
    return;
}
void main()
{
    while(1)
    {
        __asm__("hlt");
    }
    return;
}
