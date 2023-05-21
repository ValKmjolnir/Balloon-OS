#ifndef __CSTRING_H__
#define __CSTRING_H__

int strcmp(const char* s1, const char* s2) {
    int ret = 0;
    while(!(ret=*s1-*s2)&&*s2) {
        ++s1,++s2;
    }
    return ret;
}

#endif