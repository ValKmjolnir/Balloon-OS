all: Image

.PHONY=clean run-qemu

system:system.s
	- @as --32 system.s -o system

system.s:head.s main.s
	- @cat head.s main.s > system.s

main.s:
	- @gcc -m32 -S main.c -o main.s

setup.o:setup.s
	- @as --32 setup.s -o setup.o

setup:setup.o
	- @ld -T ld_boot.ld setup.o -o setup
	- @objcopy -O binary -j .text setup

bootsect.o:bootsect.s
	- @as --32 bootsect.s -o bootsect.o

bootsect:bootsect.o ld_boot.ld
	- @ld -T ld_boot.ld bootsect.o -o bootsect
	- @objcopy -O binary -j .text bootsect

Image:bootsect setup system
	- @dd if=bootsect of=Image bs=512 count=1
	- @dd if=setup of=Image bs=512 count=4 seek=1
	- @dd if=system of=Image bs=512 count=10 seek=5
	- @echo "Image built done"

clean:
	- @rm -f *.o bootsect setup main.s system.s system Image

run-qemu:Image
	- @qemu-system-i386 -boot a -fda Image
