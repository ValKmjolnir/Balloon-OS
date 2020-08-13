
void __write_mem8(int addr,int data);

void main()
{
    for(;;){
        int i=0;
        for(i=0xa0000;i<=0xaffff;++i)
            __write_mem8(i,0x0a);
    }
    return;
}