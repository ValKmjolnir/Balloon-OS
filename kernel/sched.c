#ifndef __SCHED_C__
#define __SCHED_C__
#define PAGE_SIZE 4096
long user_stack[PAGE_SIZE>>2];
struct {
    long *a;
    short b;
} user_stack_start = {&user_stack[PAGE_SIZE>>2], 0x10};
#endif

