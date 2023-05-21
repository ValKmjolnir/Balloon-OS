# balloon system setup.s
# copyright @ValKmjolnir
# 2021

.code16

.equ INITSEG, 0x9000  # beginning segment of bootsect.s
.equ SETUPSEG,0x9020  # beginning segment of setup.s
.equ SYSSEG,  0x1000  # beginning segment of system(old)
.equ SYSBEG,  0x0000  # beginning segment of system(new)

setup:
    mov $INITSEG,%ax  # ax=0x9000
    mov %ax,%ds       # set ds=0x9000

# do not need this now...
# set_vga:
#   mov $0x0013,%ax
#   int $0x10         # 0xa0000~0xaffff video RAM

before_move:
    mov $SYSBEG,%ax
    mov %ax,%es       # move system module to 0x00000
    mov $SYSSEG,%ax
    mov %ax,%ds       # source address 0x10000

do_move:
    mov %es,%ax
    cmp $0x7000,%ax   # if es==0x7000 call end_move
    jz load_idt_gdt

    mov $0x8000,%cx   # repeat 0x8000
    xor %di,%di       # di=0
    xor %si,%si       # si=0
    rep               # ds:di->es:si
    movsw             # 0x8000*2=0x10000=64kb
    mov %es,%ax
    add $0x1000,%ax
    mov %ax,%es       # es+=0x1000
    add $0x1000,%ax
    mov %ax,%ds       # ds=es+0x1000
    jmp do_move

load_idt_gdt:
    mov $SETUPSEG,%ax
    mov %ax,%ds       # ds=0x9020
    lidt idt_info     # ds:idt_info
    lgdt gdt_info     # ds:gdt_info
    cli               # close interrupt

fast_a20:
    inb $0x92,%al     # 0x92 south bridge
    orb $0x02,%al     # 00000010b second bit open a20
    outb %al,$0x92    # send data to 0x92 south bridge

enable_protect_mode:
    mov %cr0,%eax     # get cr0
    bts $0,%eax       # set eax[0]=1
    mov %eax,%cr0     # set cr0=eax

    mov $0x10,%ax     # 00010(index 2) 0(TI 0 GDT) 00 (level[highest])
    mov %ax,%ds       # TI 1 LDT
    mov %ax,%es
    mov %ax,%fs
    mov %ax,%gs
    ljmp $0x8,$0x0    # 00001(index 2) 0(TI 0 GDT) 00 (level[highest])

.align 2
.word 0

idt_info:
    .word 0           # IDT size 0 byte
    .word 0,0         # IDT base 0<<16+0=0

.align 2
.word 0

gdt_info:
    .word 0x800       # DGT size 2048 bytes -> 256 GDT entries (8 bytes)
    .word 512+gdt,0x9 # GDT base: 0x9<<16+0x200+gdt=0x90200(setup segment)+gdt

.align 8

gdt:
    # index 0
    .word 0,0,0,0     # dummy(8 bytes) without this CPU will shutdown this program(error 13)

    # index 1 system program segment
    .word 0x07ff      # system program limit 0x0800*0x1000=8MB
    .word 0x0000      # system program base address
    .word 0x9a00      # P=1(segment exist) DPL=00(0 level) S=1(code/data segment) TYPE=1010(read/exec) base=0x00
    .word 0x00c0      # base=0x00 G=1(4kb) D/B=1(32bit system) L=0(L is reserved for 64bit system) AVL=0

    # index 2 system data segment
    .word 0x07ff      # system data limit 0x0800*0x1000=8MB
    .word 0x0000      # system data base address
    .word 0x9200      # P=1(segment exist) DPL=00(0 level) S=1(code/data segment) TYPE=0010(read/write) base=0x00
    .word 0x00c0      # base=0x00 G=1(4kb) D/B=1(32bit system) L=0(L is reserved for 64bit system) AVL=0

