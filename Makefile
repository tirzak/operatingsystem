GCCPARAMS = -m32 -ffreestanding
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = loader.o gdt.o kernel.o

%.o: %.c
	gcc $(GCCPARAMS) -o $@ -c $<

%.o: %.s
	as $(ASPARAMS) -o $@ -c $<

mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin

mykernel.iso: mykernel.bin
				mkdir iso
				mkdir iso/boot
				mkdir iso/boot/grub
				cp $< iso/boot/
				echo 'set timeout=0' >> iso/boot/grub/grub.cfg
				echo 'set default=0' >> iso/boot/grub/grub.cfg
				echo '' >> iso/boot/grub/grub.cfg
				echo 'menuentry 'My Operating Sys' {' >> iso/boot/grub/grub.cfg
				echo '   multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
				echo '   boot' >> iso/boot/grub/grub.cfg
				echo '}' >> iso/boot/grub/grub.cfg
				grub-mkrescue --output=$@ iso
				rm -rf iso