# ballon system bootsect.s
# copyright @ValKmjolnir
# 2021

# boot sector 512 bytes
# loaded when starting machine
# at address 0x07c00~0x07dff

.code16

.equ BOOTSEG,  0x07c0        # i386 begins running from 0x07c0:0x0000->0x07c00
.equ INITSEG,  0x9000        # move boot to 0x9000:0x0000->0x90000
.equ SETUPSEG, 0x9020        # begin address of setup.s
.equ SYSSEG,   0x1000        # begin address of system module
.equ SYSEND,   0x9000        # end address of system module 512k

                             # cs code  segment
                             # ds data  segment
                             # es extra segment
                             # ss stack segment
                             # fs add in 80386
                             # gs add in 80386
                             # sp stack pointer
lets_begin:
    ljmp $BOOTSEG,$bootstart # cs:ip ($BOOTSEG<<4)+$bootstart=0x07c00+bootstart
                             # this means jump to bootstart function below
                             # and set cs:ip registers

bootstart:
    mov $BOOTSEG,%ax         # ax=0x07c0
    mov %ax,%ds              # ds:di (0x07c0<<4)+0
    mov $INITSEG,%ax         # ax=0x9000
    mov %ax,%es              # es:si (0x9000<<4)+0
    xor %di,%di              # di=0
    xor %si,%si              # si=0

    mov $0x100,%cx           # movsw:cx(0x100) word from ds:di(0x07c00) to es:si(0x90000)
    rep                      # this will repeat 256 times
    movsw                    # each time moving 2 bytes from 0x07c00+i*2 to 0x90000+i*2
                             # this is like these codes in C:
                             # for(int i=0;i<256;++i){
                             #    (0x90000)[i*2]=(0x07c00)[i*2];
                             #    (0x90000)[i*2+1]=(0x07c00)[i*2+1];
                             # }
                             # after this the boot sector is set at 0x90000~0x901ff

    ljmp $INITSEG,$stackset  # cs:ip ($INITSEG<<4)+$stackset=0x90000+stackset

stackset:
    mov %cs,%ax              # ax=INITSEG=0x9000
    mov %ax,%ds              # ds=ax     =0x9000
    mov %ax,%es              # es=ax     =0x9000
    mov %ax,%ss              # ss=ax     =0x9000
    mov %ax,%fs              # fs=ax     =0x9000
    mov %ax,%gs              # gs=ax     =0x9000
    mov $0xff00,%sp          # set stack pointer
                             # sp=0xff00 ss:sp ($INITSEG<<4)+0xff00
                             # ss:sp=0x90000+0xff00=0x9ff00

start:
    mov $0x01,%ah
    mov $0x20,%ch            # 0010 hide cursor
    int $0x10                # BIOS video service

load_setup:
    mov $0x0000,%dx          # dh (head 0x00)  dl (drive 0x00[floppy])
    mov $0x0002,%cx          # ch (track 0x00) cl (begin from sector 0x02)
    mov $0x0204,%ax          # ah (read 0x02)  al (read 0x04 sectors)
    mov $0x0200,%bx          # read setup to es:bx (0x9000<<4)+0x0200=0x90200
    int $0x13                # BIOS drive service
    jnc load_system          # carray flag will be 1 if failed
    
    mov $0x0000,%dx
    mov $0x0000,%ax          # ah (0x00 reset drive)
    int $0x13                # BIOS drive service
    jmp load_setup           # if failed,try again

sectors:
    .word 0                  # store the number of sectors in each track

load_system:
    mov $0x0000,%dx          # dh (head 0x00) dl (drive 0x00[floppy])
    mov $0x0800,%ax          # ah (drive parameters 0x08)
    int $0x13                # es:si = address points to parameter list
    mov %cx,%cs:sectors+0    # store number of sectors of each track in cs:sectors

before_load:
    xor %bx,%bx              # bx=0
    mov $0x0000,%dx          # dh (head 0x00)  dl (drive 0x00[floppy])
    mov $0x0005,%cx          # ch (track 0x00) cl (begin from sector 0x05)
    mov $SYSSEG,%ax          # ax=0x1000
    mov %ax,%es              # es:bx=0x10000

check:
    mov %cs:sectors+0,%ax    # ax=track number
    cmpb %al,%cl             # update dh(head) and ch(track) when cl==al
    jne load_process         # if(al!=cl) load_process
    cmpb $0x01,%dh           # else if(dh==0x01) ++ch (track)
    jne new_sector
    addb $0x01,%ch

new_sector:
    xorb $0x01,%dh           # dh=(dh==1)?0:1 (head)
    xorb %cl,%cl             # cl=0 (sector)
    
load_process:
    addb $0x01,%cl           # add 1 to cl(sector index 1~sector_number)
    mov $0x0201,%ax          # ah (0x02 read) al (0x01 sector)
    int $0x13                # read it
    mov %es,%ax
    add $0x20,%ax
    mov %ax,%es              # add 0x20 to es,so es:bx+=0x200=512 bytes
    cmpw $SYSEND,%ax
    je load_success          # if(ax==SYSEND) load_success
    jmp check                # else check

load_success:
    ljmp $SETUPSEG,$0        # cs:ip=0x90200

.=510
signature:
    .word 0xaa55             # magic number for MBR
