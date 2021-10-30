objects = loader.o kernel.o drivers.o gdt.o port.o interrupt_stubs.o interrupt.o streamio.o keyboard.o mouse.o base_string.o

%.o: %.cpp
	g++ -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -fno-stack-protector -fpermissive -o $@ -c $< -g

%.o: %.asm
	nasm -f elf32 -o $@ $<

%.o: %.s
	as --32 -o $@ $<

run: mykernel.iso
	(killall VirtualBox && sleep 1) || true
	VirtualBox --startVM 'Minor Project' &

mykernel.bin: linker.ld ${objects}
	ld -m elf_i386 -T $< -o $@ ${objects}

qemu: mykernel.bin
	qemu-system-x86_64 -kernel $<

mykernel.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot
	echo 'set timeout=0' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

.PHONY: clean
clean:
	rm -rf ${objects} mykernel.bin mykernel.iso
