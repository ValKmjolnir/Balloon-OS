# ballon system bootsect.s
# copyright @ValKmjolnir

.code16
.global bootstart
.global databeg,dataend,bssbeg,bssend,textbeg,textend

.text
textbeg:
.data
databeg:
.bss
bssbeg:
.text

.equ BOOTSEG, 0x07c0
# begin from 0x07c0:0x0000->0x07c00
.equ INITSEG, 0x9000
# jump to 0x9000:0x0000->0x90000
.equ DEMOSEG, 0x1000


ljmp $BOOTSEG,$bootstart

bootstart:
	mov $BOOTSEG,%ax
	mov %ax,%ds
	mov $INITSEG,%ax
	mov %ax,%es
	xor %di,%di
	xor %si,%si
	mov $0x100,%cx
	rep
	movsw
	ljmp $INITSEG,$stackset
	# copy operation use rep movsw 256(cx) times to move 512 bytes from ds:di to es:si
	# copy bootsect from 0x07c00 to 0x90000 and jump to 0x90000+stackset
stackset:
	mov %cs,%ax # ax=INITSEG
	mov %ax,%ds # ds=ax
	mov %ax,%es # es=ax
	mov %ax,%ss # ss=ax
	mov $0xff00,%sp # set sp=0xff00

load_setup:
	mov $0x0000,%dx # DH(head) DL(drive)
	mov $0x0002,%cx # CH(track) CL(sector)
	mov $0x0204,%ax # AH(0x02 read) AL(4 sectors)
	mov $0x0200,%bx # es is alreay 0x9000,file will be readed to 0x9000:0x0200->0x90200
	int $0x13       # BIOS drive service
	jnc printinfo   # if read failed carry flag will be 1
	mov $0x0000,%dx
	mov $0x0000,%ax
	int $0x13        # reset
	jmp load_setup

printinfo:
	mov $0x03,%ah # read cursor position
	xor %bh,%bh   # set page 0
	int $0x10
	
	mov $BOOTSEG,%ax
	mov %ax,%es

	mov $startstr,%bp
	mov $0x1301,%ax
	mov $0x0007,%bx
	mov $30,%cx
	int $0x10
	# print start string

loop:
	hlt
	jmp loop

startstr:
	.ascii "Starting Balloon System..."
	.byte 13,10,13,10

.=510
signature:
	.word 0xaa55
.text
textend:
.data
dataend:
.bss
bssend:
