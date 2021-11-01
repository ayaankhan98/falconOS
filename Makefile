objects = bin/loader.o 								\
	  bin/kernel.o 								\
	  bin/gdt.o 								\
		bin/resources/multitasking.o \
		bin/resources/dynamic_memory_management.o   \
	  bin/drivers/drivers.o 						\
	  bin/drivers/keyboard.o 						\
	  bin/drivers/mouse.o 							\
	  bin/hardware_interaction/port.o 					\
	  bin/hardware_interaction/interrupt_stubs.o 				\
	  bin/hardware_interaction/interrupt.o 					\
	  bin/hardware_interaction/pci.o 					\
	  bin/core/streamio.o 							\
	  bin/core/base_string.o

bin/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -fno-stack-protector -fpermissive -o $@ -c $< -g

bin/%.o: src/%.asm
	mkdir -p $(@D)
	nasm -f elf32 -o $@ $<

bin/%.o: src/%.s
	mkdir -p $(@D)
	as --32 -o $@ $<

run: kernel.iso
	(killall VirtualBox && sleep 1) || true
	VirtualBox --startVM 'Minor Project' &

kernel.bin: linker.ld ${objects}
	ld -m elf_i386 -T $< -o $@ ${objects}

qemu: kernel.bin
	qemu-system-x86_64 -kernel $<

kernel.iso: kernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot
	echo 'set timeout=0' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/kernel.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

.PHONY: clean
clean:
	rm -rf bin kernel.bin kernel.iso
