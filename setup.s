# balloon system setup.s
# copyright @ValKmjolnir
# this is only a demo now,i'll change this file later
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
# beginning address of system

setup:
	mov $INITSEG,%ax
	mov %ax,%ds     # set ds=0x9000

	mov $0x03,%ah   # read cursor position
	xor %bh,%bh     # set page 0
	int $0x10       # line and column number will be stored in dx,DH=line,DL=column

	mov %dx,%ds:0   # store dx to 0x90000,2 bytes

	mov $SETUPSEG,%ax
	mov %ax,%es      # set es=SETUPSEG to use es:bp points to info string

	mov $info,%bp    # set string address
	mov $0x1301,%ax  # write string,move cursor
	mov $0x0007,%bx  # page 0,black background/white characters
	mov $31,%cx      # length of string
	int $0x10        # BIOS video service

load_idt_gdt:
	mov $SETUPSEG,%ax
	mov %ax,%ds
	lidt idt_info # ds:idt_info
	lgdt gdt_info # ds:gdt_info
	cli # close interrupt

a20_on:
	call empty_8042
	movb $0xd1,%al
	outb $0x64     # send write command to 8042 output port

	call empty_8042
	movb $0xdf,%al # A20 on
	outb $0x60

	call empty_8042
	jmp enable_p_mode

empty_8042:
	inb $0x64      # 0x64 8042 status port
	testb %al,0x02 # is input buffer full?
	jnz empty_8042
	ret
# in protect mode cs,ds,es,ss... store the index of gdt/idt and the gdt/idt mode
# example cs[2]=1 means using idt cs[1:0] means RPL(Requested Privilege Level)

enable_p_mode:
	mov $0x0001,%ax # change PE bit to 1
	lmsw %ax
	ljmp $8,$0
# cs (binary)00000000 00001 0 00 (index 1 in gdt(0) 0 level[highest])
# eip=0

info:
	.ascii "You are now in setup process."
	.byte 13,10

gdt:
	.word 0,0,0,0 # dummy(8 bytes) without this CPU will shutdown this program(error 13)
	# system program segment
	.word 0x07ff # system program limit 0x0800*0x1000=8MB
	.word 0x1000 # system program base address
	.word 0x9a00 # P=1(segment exist) DPL=00(0 level) S=1(code/data segment) TYPE=1010(read/exec) base=0x00
	.word 0x00c0 # base=0x00 G=1(4kb) D/B=1(32bit system) L=0(L is reserved for 64bit system) AVL=0
	# system data segment
	.word 0x07ff # system program limit 0x0800*0x1000=8MB
	.word 0x1000 # system program base address
	.word 0x9200 # P=1(segment exist) DPL=00(0 level) S=1(code/data segment) TYPE=0010(read/write) base=0x00
	.word 0x00c0 # base=0x00 G=1(4kb) D/B=1(32bit system) L=0(L is reserved for 64bit system) AVL=0

idt_info:
	.word 0   # idt limit 0
	.word 0,0 # idt base: 0<<16+0
# IDTR 48 bit
# IDTR[47:16] base address,IDTR[15:0] idt limit
gdt_info:
	.word 0x800 # 2048 bytes -> 256 GDT entries (8 bytes)
	.word 512+gdt,0x9 # gdt base: 0x9<<16+0x200+gdt=0x90200+gdt
# GDTR 48 bit
# GDTR[47:16] base address,GDTR[15:0] gdt limit
# gdt_info means GDTR=0x9,0x200+gdt,0x0800 (little-endian)
# then GDTR=0x90200+gdt(47:16),0x800(15:0)
.text
textend:
.data
dataend:
.bss
bssend:
