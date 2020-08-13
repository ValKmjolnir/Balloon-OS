long user_stack[1024];
struct {
    long *a;
    short b;
} stack_start = {&user_stack[1024], 0x10};