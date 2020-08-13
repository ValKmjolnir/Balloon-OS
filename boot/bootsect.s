# ballon system bootsect.s
# copyright @ValKmjolnir
# 2020

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
# jump to 0x9000:0x0000->0x90000 then bootsect 0x90000~0x901ff
.equ SETUPSEG,0x9020
# beginning address of setup.s
.equ SYSSEG, 0x1000
# beginning address of system
.equ SYSEND, 0x8020
# system ends at 0x70000=448kb


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
	mov %cs,%ax     # ax=INITSEG
	mov %ax,%ds     # ds=ax
	mov %ax,%es     # es=ax
	mov %ax,%ss     # ss=ax
	mov $0xff00,%sp # set sp=0xff00

load_setup:
	mov $0x0000,%dx    # DH(0 head) DL(0 drive)
	mov $0x0002,%cx    # CH(0 track) CL(from 2 sector)
	mov $0x0204,%ax    # AH(0x02 read) AL(4 sectors)
	mov $0x0200,%bx    # es is alreay 0x9000,file will be readed to 0x9000:0x0200->0x90200
	int $0x13          # BIOS drive service
	jnc get_sector_num # if read failed carry flag will be 1

	mov $0x0000,%dx
	mov $0x0000,%ax   # ah=0x00 means use BIOS reset floppy/disk
	int $0x13         # reset

get_sector_num:
	mov $0x0000,%dx # dl=0x00 means it is floppy,if it is disk,dl[7]=1
	mov $0x0800,%ax # ah=0x08 means get drive parameters
	int $0x13       # get it,after this int,es will be set to an address points to parameter list
	mov %cx,%cs:sectors+0 # store number of sectors of each track in cs:sectors

before_load_system:
	xor %bx,%bx     # set bx=0x0000
	mov $0x0000,%dx # DH(0 head) DL(0 drive[floppy])
	mov $0x0005,%cx # CH(0 track)CL(5 sector)
	mov $SYSSEG,%ax
	mov %ax,%es     # set es=0x1000

load_system:
	mov %cs:sectors+0,%ax # load sector_number to ax
	cmpb %al,%cl          # if CL(sector)==sector_number means need to update DH(head) and CH(track)
	jne load_process      # if not equal,continue loading
	cmpb $0x01,%dh        # if DH==1,CH+1
	jne set_new_dx_cx     # if not equal,skip adding 1 to CH
	addb $0x01,%ch        # add 1 to CH to change the track

set_new_dx_cx:
	xorb $0x01,%dh # change DH,DH=0x00 then DH is set to 0x01,else DH is set to 0x00
	xorb %cl,%cl

load_process:
	addb $0x01,%cl  # add 1 to CL to load the next sector
	mov $0x0201,%ax # AH(0x02 read) AL(0x01 sectors)
	int $0x13
	jc failed_load_system

	mov %es,%ax
	addw $0x20,%ax # add 0x20 to es,then es:bx is added 0x200=512 bytes
	mov %ax,%es

	cmpw $SYSEND,%ax # if es==SYSEND,end loading and print info string
	je ok_load_system # SYSEND(0x4220) is the condition to stop loading,system will be loaded from 0x10000 to (0x4220-0x20)<<4
	jmp load_system # if es!=SYSEND then continue loading

ok_load_system:
	mov $0x03,%ah   # read cursor position
	xor %bh,%bh     # set page 0
	int $0x10       # BIOS video service

	mov $INITSEG,%ax
	mov %ax,%es     # es:bp points to the string

	mov $sysmsg,%bp # set string address
	mov $0x1301,%ax # write string,move cursor
	mov $0x0007,%bx # page 0,black background/white characters
	mov $28,%cx     # length of string
	int $0x10       # BIOS video service

	ljmp $SETUPSEG,$0 # jump to setup

failed_load_system:
	mov $0x0000,%dx
	mov $0x0000,%ax   # ah=0x00 means use BIOS reset floppy/disk
	int $0x13         # reset

	mov $0x03,%ah   # read cursor position
	xor %bh,%bh     # set page 0
	int $0x10

	mov $INITSEG,%ax
	mov %ax,%es      # es:bp points to the string

	mov $failmsg,%bp # set string address
	mov $0x1301,%ax  # write string,move cursor
	mov $0x0007,%bx  # page 0,black background/white characters
	mov $24,%cx      # length of string
	int $0x10        # 0x10 video service

die:
	hlt
	jmp die # if failed to load the system,enter this infinite loop

sysmsg:
	.ascii "Starting Balloon System..."
	.byte 13,10

failmsg:
	.ascii "Loading System failed."
	.byte 13,10

sectors:
	.word 0
.=510
signature:
	.word 0xaa55
.text
textend:
.data
dataend:
.bss
bssend:
