#!/bin/bash
~/implementations/qemu_x86/qemu/build/qemu-system-i386 -cdrom os.iso \
	-chardev stdio,id=characted_dev \
        -device isa-debugcon,iobase=0x3f8,chardev=characted_dev   -d mmu

	

