# still some bugs inside,i'm trying to fix them
.code32
.text
.global idt,gdt,pg_dir,startup_32,__write_mem8

pg_dir:

startup_32:
    movl $0x10,%eax
	mov %ax,%ds
	mov %ax,%es
	mov %ax,%fs
	mov %ax,%gs
	lss stack_start,%esp
	call setup_idt
	call setup_gdt
	movl $0x10,%eax # reload all the segment registers
	mov %ax,%ds	    # after changing gdt. CS was already
	mov %ax,%es	    # reloaded in 'setup_gdt'
	mov %ax,%fs
	mov %ax,%gs
	lss stack_start,%esp
	xorl %eax,%eax
check:
	incl %eax		   # check that A20 really IS enabled
	movl %eax,0x000000 # loop forever if it isn't
	cmpl %eax,0x100000
	je check
	
	movl %cr0,%eax
	andl $0x80000011,%eax
	orl  $2,%eax # set MP Bit
	movl %eax,%cr0
	call check_x87
	jmp sys_start

check_x87:
	fninit
	fstsw %ax
	cmpb $0,%al
	je 1f
	movl %cr0,%eax
	xorl $6,%eax
	movl %eax,%cr0
	ret
.align 2
1:	.byte 0xdb,0xe4 # 287
	ret

sys_start:
    pushl $0
    pushl $0
    pushl $0
    pushl $loop
	pushl $main
    jmp setup_paging
loop:
    jmp loop

setup_paging:
	ret

setup_idt:
	lidt idt_descr
	ret

setup_gdt:
	lgdt gdt_descr
	ret

__write_mem8:
	mov 8(%esp),%al
	mov %al,4(%esp)
	ret

idt_descr:
	.word 256*8-1
	.long idt

gdt_descr:
	.word 256*8-1
	.long gdt

idt:
    .fill 256,8,0		# idt is uninitialized

gdt:
    .quad 0x0000000000000000 /* NULL descriptor */
	.quad 0x00c09a0000000fff /* 16Mb */
	.quad 0x00c0920000000fff /* 16Mb */
	.quad 0x0000000000000000 /* TEMPORARY - don't use */
	.fill 252,8,0            /* space for LDT's and TSS's etc */

