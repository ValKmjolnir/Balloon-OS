long user_stack[1024];
struct{
    long *stack_pointer;
    short base;
}stack_start={&user_stack[1024],0x10};

void main()
{
    for(;;){
        long* i=0;
        for(i=0xa0000;i<=0xaffff;++i)
            *i=0;
    }
    return;
}