All: Image

.PHONY=clean run-qemu

system:boot/head.o init/main.o kernel/sched.o kernel/printk.o
	- @ld -T ld_sys.ld -m elf_i386 boot/head.o init/main.o kernel/sched.o kernel/printk.o -o system.sym
	- @strip system.sym -o system.o
	- @objcopy -O binary -R .note -R .comment system.o system
# use -m elf_i386 to generate elf-i386 executable file
# GNU strip discards all symbols from object files objfile

boot/head.o:boot/head.s
	- @as --32 boot/head.s -o boot/head.o
# head.o will be linked with init/main.o and kernel by ld

boot/setup:boot/setup.s
	- @as --32 boot/setup.s -o boot/setup.o
	- @ld -T boot/ld_boot.ld boot/setup.o -o boot/setup
	- @objcopy -O binary -j .text boot/setup
# code16 start up progress

boot/bootsect:boot/bootsect.s boot/ld_boot.ld
	- @as --32 boot/bootsect.s -o boot/bootsect.o
	- @ld -T boot/ld_boot.ld boot/bootsect.o -o boot/bootsect
	- @objcopy -O binary -j .text boot/bootsect
# code16 bootloader

init/main.o:init/main.c
	- @gcc -m32 -c init/main.c -fno-stack-protector -o init/main.o

kernel/sched.o:kernel/sched.c
	- @gcc -m32 -c kernel/sched.c -fno-stack-protector -o kernel/sched.o

kernel/printk.o:kernel/printk.c
	- @gcc -m32 -c kernel/printk.c -fno-stack-protector -o kernel/printk.o

Image:boot/bootsect boot/setup system
	- @dd if=boot/bootsect of=Image bs=512 count=1
	- @dd if=boot/setup of=Image bs=512 count=4 seek=1
	- @dd if=system of=Image bs=512 count=1440 seek=5
	- @dd if=/dev/zero of=Image bs=512 count=1435 seek=1445
	- @echo "Image built done"
# Image of this system

clean:
	- @rm -f *.o system.sym system Image
	- @rm -f boot/*.o boot/bootsect boot/setup
	- @rm -f init/*.o init/*.s
	- @rm -f kernel/*.o kernel/*.s

run-qemu:Image
	- @qemu-system-i386 -boot a -fda Image
# -fda running from virtual floppy
