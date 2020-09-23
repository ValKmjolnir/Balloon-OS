#ifndef PRT_CHAR_H
#define PRT_CHAR_H

#define setpixel(base,x,y,color) (*(char*)(base+(x)+(y)*320)=color)

void print_right_arrow(int base)
{
    setpixel(base,0,1,0);
    setpixel(base,1,1,0);
    setpixel(base,2,2,0);
    setpixel(base,3,3,0);
    setpixel(base,2,4,0);
    setpixel(base,1,5,0);
    setpixel(base,0,5,0);
    return;
}

void print_a(int base)
{
    int i;
    for(i=2;i<6;++i)
    {
        setpixel(base,1,i,0);
        setpixel(base,4,i,0);
    }
    for(i=2;i<4;++i)
    {
        setpixel(base,i,1,0);
        setpixel(base,i,4,0);
    }
    return;
}
void print_b(int base)
{
    int i;
    for(i=1;i<6;++i)
        setpixel(base,1,i,0);
    for(i=2;i<4;++i)
    {
        setpixel(base,i,1,0);
        setpixel(base,i,3,0);
        setpixel(base,i,5,0);
    }
    setpixel(base,4,2,0);
    setpixel(base,4,4,0);
    return;
}
void print_c(int base)
{
    int i;
    for(i=2;i<5;++i)
    {
        setpixel(base,i,1,0);
        setpixel(base,i,5,0);
    }
    for(i=2;i<5;++i)
        setpixel(base,1,i,0);
    return;
}
void print_d(int base)
{
    int i;
    for(i=2;i<4;++i)
    {
        setpixel(base,i,1,0);
        setpixel(base,i,5,0);
    }
    for(i=1;i<6;++i)
        setpixel(base,1,i,0);
    for(i=2;i<5;++i)
        setpixel(base,4,i,0);
    return;
}
void print_e(int base)
{
    int i;
    for(i=1;i<6;++i)
        setpixel(base,1,i,0);
    for(i=2;i<4;++i)
    {
        setpixel(base,i,1,0);
        setpixel(base,i,3,0);
        setpixel(base,i,5,0);
    }
    return;
}
void print_f(int base)
{
    int i;
    for(i=1;i<6;++i)
        setpixel(base,1,i,0);
    for(i=2;i<4;++i)
    {
        setpixel(base,i,1,0);
        setpixel(base,i,3,0);
    }
    return;
}
void print_g(int base)
{
    int i;
    for(int i=2;i<5;++i)
    {
        setpixel(base,i,1,0);
        setpixel(base,i,5,0);
        setpixel(base,1,i,0);
    }
    setpixel(base,3,3,0);
    setpixel(base,4,3,0);
    setpixel(base,4,4,0);
    return;
}
void print_h(int base)
{
    int i;
    for(i=1;i<6;++i)
    {
        setpixel(base,1,i,0);
        setpixel(base,4,i,0);
    }
    setpixel(base,2,3,0);
    setpixel(base,3,3,0);
    return;
}
void print_i(int base)
{
    int i;
    for(i=1;i<4;++i)
    {
        setpixel(base,i,1,0);
        setpixel(base,i,5,0);
    }
    for(i=2;i<5;++i)
        setpixel(base,2,i,0);
    return;
}
void print_j(int base)
{
    int i;
    for(i=1;i<5;++i)
        setpixel(base,4,i,0);
    setpixel(base,3,1,0);
    setpixel(base,3,5,0);
    setpixel(base,2,5,0);
    setpixel(base,1,4,0);
    return;
}
void print_k(int base)
{
    int i;
    for(int i=1;i<6;++i)
        setpixel(base,1,i,0);
    setpixel(base,2,3,0);
    setpixel(base,3,2,0);
    setpixel(base,3,4,0);
    setpixel(base,4,1,0);
    setpixel(base,4,5,0);
    return;
}
void print_l(int base)
{
    int i;
    for(i=1;i<6;++i)
        setpixel(base,1,i,0);
    setpixel(base,2,5,0);
    setpixel(base,3,5,0);
    return;
}
void print_m(int base)
{
    int i;
    for(i=1;i<6;++i)
    {
        setpixel(base,0,i,0);
        setpixel(base,4,i,0);
    }
    setpixel(base,1,2,0);
    setpixel(base,2,3,0);
    setpixel(base,3,2,0);
    return;
}
void print_n(int base)
{
    int i;
    for(i=1;i<6;++i)
    {
        setpixel(base,1,i,0);
        setpixel(base,4,i,0);
    }
    setpixel(base,2,2,0);
    setpixel(base,3,3,0);
    return;
}
void print_o(int base)
{
    int i;
    for(i=2;i<5;++i)
    {
        setpixel(base,1,i,0);
        setpixel(base,4,i,0);
    }
    for(i=2;i<4;++i)
    {
        setpixel(base,i,1,0);
        setpixel(base,i,5,0);
    }
    return;
}
void print_p(int base)
{
    int i;
    for(i=0;i<5;++i)
        setpixel(base,1,i,0);
    for(i=2;i<4;++i)
    {
        setpixel(base,i,0,0);
        setpixel(base,i,3,0);
        setpixel(base,4,i-1,0);
    }
    return;
}
void print_q(int base)
{
    int i;
    for(i=2;i<5;++i)
    {
        setpixel(base,1,i,0);
        setpixel(base,4,i,0);
    }
    for(i=2;i<4;++i)
    {
        setpixel(base,i,1,0);
        setpixel(base,i,5,0);
    }
    setpixel(base,3,4,0);
    setpixel(base,5,5,0);
    return;
}
void print_r(int base)
{
    int i;
    for(i=1;i<6;++i)
        setpixel(base,1,i,0);
    for(i=2;i<4;++i)
    {
        setpixel(base,i,1,0);
        setpixel(base,i,4,0);
        setpixel(base,4,i,0);
    }
    setpixel(base,4,5,0);
    return;
}
void print_s(int base)
{
    int i;
    for(i=1;i<4;++i)
    {
        setpixel(base,i+1,1,0);
        setpixel(base,i,5,0);
    }
    setpixel(base,1,2,0);
    setpixel(base,2,3,0);
    setpixel(base,3,3,0);
    setpixel(base,4,4,0);
    return;
}
void print_t(int base)
{
    int i;
    for(i=1;i<4;++i)
        setpixel(base,i,1,0);
    for(i=2;i<6;++i)
        setpixel(base,2,i,0);
    return;
}
void print_u(int base)
{
    int i;
    for(i=1;i<5;++i)
    {
        setpixel(base,1,i,0);
        setpixel(base,4,i,0);
    }
    setpixel(base,2,5,0);
    setpixel(base,3,5,0);
    return;
}
void print_v(int base)
{
    int i;
    for(i=1;i<5;++i)
        setpixel(base,1,i,0);
    setpixel(base,2,5,0);
    setpixel(base,3,3,0);
    setpixel(base,3,4,0);
    setpixel(base,4,1,0);
    setpixel(base,4,2,0);
    return;
}
void print_w(int base)
{
    int i;
    for(i=1;i<5;++i)
    {
        setpixel(base,0,i,0);
        setpixel(base,4,i,0);
    }
    for(i=2;i<5;++i)
        setpixel(base,2,i,0);
    setpixel(base,1,5,0);
    setpixel(base,3,5,0);
    return;
}
void print_x(int base)
{
    int i;
    for(i=1;i<3;++i)
    {
        setpixel(base,1,i,0);
        setpixel(base,1,i+3,0);
        setpixel(base,4,i,0);
        setpixel(base,4,i+3,0);
        setpixel(base,i+1,3,0);
    }
    return;
}
void print_y(int base)
{
    int i;
    for(i=1;i<3;++i)
    {
        setpixel(base,1,i,0);
        setpixel(base,4,i,0);
        setpixel(base,4,i+2,0);
        setpixel(base,i+1,3,0);
        setpixel(base,i+1,5,0);
    }
    setpixel(base,4,4,0);
    return;
}
void print_z(int base)
{
    int i;
    for(i=1;i<4;++i)
    {
        setpixel(base,i,1,0);
        setpixel(base,i,5,0);
    }
    setpixel(base,1,4,0);
    setpixel(base,2,3,0);
    setpixel(base,3,2,0);
    return;
}

void print_char(char c,int base)
{
    switch(c)
    {
        case '>':print_right_arrow(base);break;
        case 'A':case 'a':print_a(base);break;
        case 'B':case 'b':print_b(base);break;
        case 'C':case 'c':print_c(base);break;
        case 'D':case 'd':print_d(base);break;
        case 'E':case 'e':print_e(base);break;
        case 'F':case 'f':print_f(base);break;
        case 'G':case 'g':print_g(base);break;
        case 'H':case 'h':print_h(base);break;
        case 'I':case 'i':print_i(base);break;
        case 'J':case 'j':print_j(base);break;
        case 'K':case 'k':print_k(base);break;
        case 'L':case 'l':print_l(base);break;
        case 'M':case 'm':print_m(base);break;
        case 'N':case 'n':print_n(base);break;
        case 'O':case 'o':print_o(base);break;
        case 'P':case 'p':print_p(base);break;
        case 'Q':case 'q':print_q(base);break;
        case 'R':case 'r':print_r(base);break;
        case 'S':case 's':print_s(base);break;
        case 'T':case 't':print_t(base);break;
        case 'U':case 'u':print_u(base);break;
        case 'V':case 'v':print_v(base);break;
        case 'W':case 'w':print_w(base);break;
        case 'X':case 'x':print_x(base);break;
        case 'Y':case 'y':print_y(base);break;
        case 'Z':case 'z':print_z(base);break;
    }
    return;
}
void print(const char* buf,int place)
{
    char* p=buf;
    int ptr=place;
    for(;*p;++p)
    {
        print_char(*p,ptr);
        ptr+=5;
    }
    return;
}
#endif
