All: build/image.img

.PHONY=clean run-qemu

BUILD_DIR = build

build/system: build/head.o build/main.o build/sched.o build/vga_color_printk.o
	- @ld -T ld_sys.ld -m elf_i386 build/head.o build/main.o build/sched.o build/vga_color_printk.o -o build/system.sym
	- @strip build/system.sym -o build/system.o
	- @objcopy -O binary -R .note -R .comment build/system.o build/system
# use -m elf_i386 to generate elf-i386 executable file
# GNU strip discards all symbols from object files objfile

build/head.o: boot/head.s
	- @as --32 boot/head.s -o build/head.o
# head.o will be linked with init/main.o and kernel by ld

build/setup: boot/setup.s
	- @as --32 boot/setup.s -o build/setup.o
	- @ld -T boot/ld_boot.ld build/setup.o -o build/setup
	- @objcopy -O binary -j .text build/setup
# code16 start up progress

build/bootsect: boot/bootsect.s boot/ld_boot.ld
	- @as --32 boot/bootsect.s -o build/bootsect.o
	- @ld -T boot/ld_boot.ld build/bootsect.o -o build/bootsect
	- @objcopy -O binary -j .text build/bootsect
# code16 bootloader

build/main.o: init/main.c kernel/cstring.h kernel/printk.h kernel/input.h
	- @gcc -m32 -c init/main.c -fno-stack-protector -o build/main.o

build/sched.o: kernel/sched.c
	- @gcc -m32 -c kernel/sched.c -fno-stack-protector -o build/sched.o

build/vga_color_printk.o: kernel/vga_color_printk.c
	- @gcc -m32 -c kernel/vga_color_printk.c -fno-stack-protector -o build/vga_color_printk.o

build/image.img: build/bootsect build/setup build/system
	- @dd if=build/bootsect of=build/image.img bs=512 count=1
	- @dd if=build/setup of=build/image.img bs=512 count=4 seek=1
	- @dd if=build/system of=build/image.img bs=512 count=1440 seek=5
	- @dd if=/dev/zero of=build/image.img bs=512 count=1435 seek=1445
	- @echo "Image built done"
# Image of this system

clean:
	- @rm -f build/*.o build/bootsect build/setup build/system build/system.sym build/Image

run-qemu: build/image.img
	- @qemu-system-i386 -boot a -drive format=raw,file=build/image.img,index=0,if=floppy -curses
