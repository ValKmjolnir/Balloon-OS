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

.equ SETUPSEG,0x9020
# beginning address of setup.s

setup:
	hlt
	mov $SETUPSEG,%ax
	mov %ax,%es      # set es=SETUPSEG to use es:bp points to info string

	mov $info,%bp    # set string address
	mov $0x1301,%ax  # write string,move cursor
	mov $0x0007,%bx  # page 0,black background/white characters
	mov $31,%cx      # length of string
	int $0x10        # 0x10 video service
	jmp setup

info:
	.ascii "You are now in setup process."
	.byte 13,10

.text
textend:
.data
dataend:
.bss
bssend:
