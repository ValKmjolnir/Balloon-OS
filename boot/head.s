# balloon system head.s
# copyright @ValKmjolnir
# 2021

.global startup_32
.global gdt
.global idt

startup_32:
    lss stack_start,%esp   # stack_start(in init/main.c)
    lidt idt_info          # load idt
    lgdt gdt_info          # load gdt
    mov $0x10,%eax         # reload all segment registers after changing gdt
    mov %ax,%ds
    mov %ax,%es
    mov %ax,%fs
    mov %ax,%gs
    lss stack_start,%esp   # reload esp

setup_idt:
    lea ignore_int,%edx    # edx=ignore_int address
    movl $0x00080000,%eax  # 0x0008 segment selector
    movw %dx,%ax           # ax=offset_low
    mov $0x8e00,%dx        # P=1(segment exist) DPL=00(highest privilege) 01110(interrupt gate)

    lea idt,%edi           # edi=idt address
    mov $256,%cx           # repeat 256 times
rp_sidt:
    mov %eax,(%edi)        # load low 4 bytes
    mov %edx,4(%edi)       # load high 4 bytes
    addl $8,%edi           # edi+=8
    dec %cx                # --cx
    jne rp_sidt

system_main:
    push $0
    push $0
    push $0
    pushl $L
    pushl $main            # main address on top of stack
    ret                    # jump to main
L:  jmp L

int_msg:
    .asciz "unknown interrupt\n"

.align 2

ignore_int:
    pushl %eax
    pushl %ecx
    pushl %edx
    push %ds
    push %es
    push %fs
    movl $0x10,%eax
    mov %ax,%ds
    mov %ax,%es
    mov %ax,%fs
    pushl $int_msg
    call printk	
    popl %eax
    pop %fs
    pop %es
    pop %ds
    popl %edx
    popl %ecx
    popl %eax
    iret

.align 2
.word 0

idt_info:
    .word 256*8-1 # Length in Bytes - 1
    .long idt     # Base

.align 2
.word 0

gdt_info:
    .word 256*8-1
    .long gdt

.align 8

idt:
    .fill 256,8,0

gdt:
    # empty
    .quad 0x0000000000000000

    # code segment
    .quad 0x00c09a0000000fff
    # base=0x00000000
    # limit=0xfff G=1 4kb unit 0x1000*0x1000=16MB
    # type=0x2(read/write)
    # DPL=0x00(highest privilege) S=1(code/data segment) P=1(segment exist)

    # data segment
    .quad 0x00c0920000000fff

    .fill 253,8,0

