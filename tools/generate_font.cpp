#include <fstream>
#include <iostream>

unsigned char font_table[256][16];

void generate_font()
{
    std::ifstream fin("hankaku.txt");
    unsigned char tmp=0;
    int cnt=0;
    int char_index=0;
    int line_index=0;
    while(!fin.eof())
    {
        char c=fin.get();
        if(c=='.')
        {
            tmp<<=1;
            ++cnt;
        }
        else if(c=='*')
        {
            tmp<<=1;
            tmp|=1;
            ++cnt;
        }
        if(cnt==8)
        {
            font_table[char_index][line_index]=tmp;
            ++line_index;
            if(line_index==16)
            {
                ++char_index;
                line_index=0;
            }
            cnt=0;
            tmp=0;
        }
    }
    std::cout<<char_index;
    fin.close();
    return;
}
char trans(int number)
{
    if(0<=number && number<=9)
        return '0'+number;
    else
        return 'a'+(number-10);
}
void output_font()
{
    std::ofstream fout("font.h");
    fout<<"#ifndef FONT_H\n#define FONT_H\n";
    fout<<"\nunsigned char font_table[256][16]={\n";
    for(int i=0;i<256;++i)
    {
        fout<<"\t{";
        for(int j=0;j<16;++j)
        {
            int high=((0xf0&font_table[i][j])>>4);
            int low=(0x0f&font_table[i][j]);
            fout<<"0x"<<trans(high)<<trans(low);
            if(j<15)
                fout<<",";
        }
        if(i<255)
            fout<<"},\n";
        else
            fout<<"}\n";
    }
    fout<<"};\n";
    fout<<"#endif\n";
    fout.close();
    return;
}

int main()
{
    generate_font();
    output_font();
    system("pause");
    return 0;
}