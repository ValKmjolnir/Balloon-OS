# balloon system setup.s
# copyright @ValKmjolnir
# this is only a demo now,i'll change this file later

.code16
.global setup
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
get_mem_size:
	mov $0x88,%ah
	int $0x15
	mov %ax,%ds:2    # store memory size from 1MB to 0x90002,2 bytes
get_video_mode:
	mov $0x0f,%ah
	int $0x10
	mov %bx,%ds:4    # BH=page
	mov %ax,%ds:6    # AL=video mode,AH=width of character columns
check_EGA_VGA:
	mov $0x12,%ah
	mov $0x10,%bl
	int $0x10
	mov %ax,%ds:8
	mov %bx,%ds:10   # BH(one color/colorful mode) BL(video memory size)
	mov %cx,%ds:12   # CH(特性连接器比特位信息),CL(视频开关设置信息)

copy_hd0_parameter:
copy_hd1_parameter:
check_disk1:

die:
	hlt
	jmp die

	cli # no interrupts allowed

info:
	.ascii "You are now in setup process."
	.byte 13,10

.text
textend:
.data
dataend:
.bss
bssend:
