# balloon system setup.s
# copyright @ValKmjolnir
# 2020
.code16
.global databeg,dataend,bssbeg,bssend,textbeg,textend

.text
textbeg:
.data
databeg:
.bss
bssbeg:
.text

.equ INITSEG, 0x9000
# beginning address of bootsect.s
.equ SETUPSEG,0x9020
# beginning address of setup.s
.equ SYSSEG,  0x1000
# beginning address of system(old)
.equ SYSBEG,  0x0000
# beginning address of system(new)

setup:
	mov $INITSEG,%ax
	mov %ax,%ds     # set ds=0x9000

	mov $0x03,%ah   # read cursor position
	xor %bh,%bh     # set page 0
	int $0x10       # line and column number will be stored in dx,DH=line,DL=column
	mov %dx,%ds:0   # store dx to 0x90000,2 bytes

set_vga:
	mov $0x0013,%ax
	int $0x10
	# 0xa0000~0xaffff video RAM

before_move:
	mov $SYSBEG,%ax
	mov %ax,%es     # move system module to 0x00000
	mov $SYSSEG,%ax
	mov %ax,%ds     # source address 0x10000

do_move:
	mov %es,%ax
	cmp $0x7000,%ax # 0x10000~0x7ffff to 0x00000~0x6ffff if es==0x7000 call end_move
	jz end_move

	mov $0x8000,%cx
	xor %di,%di
	xor %si,%si
	rep
	movsw           # 0x8000*2=0x10000=64kb
	mov %es,%ax
	add $0x1000,%ax
	mov %ax,%es
	add $0x1000,%ax
	mov %ax,%ds
	jmp do_move

end_move:
	mov $0x8,%ax
	mov %ax,%es
	mov %ax,%ds
	mov %ax,%ss
	# at this place,BIOS interrupt vector table is overwrote
load_idt_gdt:
	mov $SETUPSEG,%ax
	mov %ax,%ds
	lidt idt_info # ds:idt_info
	lgdt gdt_info # ds:gdt_info
	cli           # close interrupt

a20_on:
	inb $0x92,%al
	orb $0x02,%al
	outb %al,$0x92
# in protect mode cs,ds,es,ss... store the index of gdt/idt and the gdt/idt mode
# example cs[2]=1 means using idt cs[1:0] means RPL(Requested Privilege Level)

# enable protected mode
enable_protect_mode:
	mov %cr0,%eax
	bts $0,%eax
	mov %eax,%cr0
# jump to protected mode
	mov $0x10,%ax
	mov %ax,%ds
	mov %ax,%es
	mov %ax,%fs
	mov %ax,%gs
	ljmp $0x8,$0x0
# cs (binary)00000000 00001 0 00 (index 1 in gdt(0) 0 level[highest])
# eip=0

idt_info:
	.word 0   # idt limit 0
	.word 0,0 # idt base: 0<<16+0
# IDTR 48 bit
# IDTR[47:16] base address,IDTR[15:0] idt limit
gdt_info:
	.word 0x800 # 2048 bytes -> 256 GDT entries (8 bytes)
	.word 512+gdt,0x9 # gdt base: 0x9<<16+0x200+gdt=0x90200(setup segment)+gdt
# GDTR 48 bit
# GDTR[47:16] base address,GDTR[15:0] gdt limit
# gdt_info means GDTR=0x9,0x200+gdt,0x0800 (little-endian)
# then GDTR=0x90200+gdt(47:16),0x800(15:0)

gdt:
	# index 0
	.word 0,0,0,0 # dummy(8 bytes) without this CPU will shutdown this program(error 13)
	# index 1 system program segment
	.word 0x07ff # system program limit 0x0800*0x1000=8MB
	.word 0x0000 # system program base address
	.word 0x9a00 # P=1(segment exist) DPL=00(0 level) S=1(code/data segment) TYPE=1010(read/exec) base=0x00
	.word 0x00c0 # base=0x00 G=1(4kb) D/B=1(32bit system) L=0(L is reserved for 64bit system) AVL=0
	# index 2 system data segment
	.word 0x07ff # system data limit 0x0800*0x1000=8MB
	.word 0x0000 # system data base address
	.word 0x9200 # P=1(segment exist) DPL=00(0 level) S=1(code/data segment) TYPE=0010(read/write) base=0x00
	.word 0x00c0 # base=0x00 G=1(4kb) D/B=1(32bit system) L=0(L is reserved for 64bit system) AVL=0

.text
textend:
.data
dataend:
.bss
bssend:
