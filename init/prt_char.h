#ifndef PRT_CHAR_H
#define PRT_CHAR_H

#define setpixel(base,x,y,color) (*(char*)(base+x+y*320)=color)


void print_right_arrow(int base);
void print_a(int base);
void print_b(int base);
void print_c(int base);
void print_d(int base);
void print_e(int base);
void print_f(int base);
void print_g(int base);
void print_h(int base);
void print_i(int base);
void print_j(int base);
void print_k(int base);
void print_l(int base);
void print_m(int base);
void print_n(int base);
void print_o(int base);
void print_p(int base);
void print_q(int base);
void print_r(int base);
void print_s(int base);
void print_t(int base);
void print_u(int base);
void print_v(int base);
void print_w(int base);
void print_x(int base);
void print_y(int base);
void print_z(int base);

void print_char(char c,int base)
{
    switch(c)
    {
        case '>':print_right_arrow(base);break;
        case 'A':case 'a':print_a(base);break;
        case 'D':case 'd':print_d(base);break;
        case 'E':case 'e':print_e(base);break;
        case 'H':case 'h':print_h(base);break;
        case 'L':case 'l':print_l(base);break;
        case 'O':case 'o':print_o(base);break;
        case 'R':case 'r':print_r(base);break;
        case 'W':case 'w':print_w(base);break;
    }
    return;
}

void print_right_arrow(int base)
{
    setpixel(base,1,0,0);
    setpixel(base,2,1,0);
    setpixel(base,3,2,0);
    setpixel(base,2,3,0);
    setpixel(base,1,4,0);
    return;
}

void print_a(int base)
{
    return;
}

void print_d(int base)
{
    return;
}

void print_e(int base)
{
    int i=0;
    for(i=0;i<5;++i)
        setpixel(base,1,i,0);
    setpixel(base,2,0,0);
    setpixel(base,3,0,0);
    setpixel(base,2,2,0);
    setpixel(base,3,2,0);
    setpixel(base,2,4,0);
    setpixel(base,3,4,0);
    return;
}

void print_h(int base)
{
    int i=0;
    for(i=0;i<5;++i)
    {
        setpixel(base,1,i,0);
        setpixel(base,3,i,0);
    }
    setpixel(base,2,2,0);
    return;
}

void print_l(int base)
{
    int i=0;
    for(i=0;i<5;++i)
        setpixel(base,1,i,0);
    setpixel(base,2,4,0);
    setpixel(base,3,4,0);
    return;
}

void print_o(int base)
{
    int i=0;
    for(i=0;i<5;++i)
    {
        setpixel(base,1,i,0);
        setpixel(base,3,i,0);
    }
    setpixel(base,2,0,0);
    setpixel(base,2,4,0);
    return;
}

void print_r(int base)
{
    return;
}

void print_w(int base)
{
    return;
}

#endif
