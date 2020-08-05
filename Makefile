all: Image

.PHONY=clean run-qemu

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
Image:bootsect setup
	- @dd if=bootsect of=Image bs=512 count=1
	- @dd if=setup of=Image bs=512 count=4 seek=1
	- @echo "Image built done"
clean:
	- @rm -f *.o bootsect setup Image
run-qemu:Image
	- @qemu-system-i386 -boot a -fda Image
