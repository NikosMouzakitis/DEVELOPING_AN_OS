#!/bin/bash
qemu-system-i386 -cdrom os.iso \
	-chardev stdio,id=characted_dev \
        -device isa-debugcon,iobase=0x3f8,chardev=characted_dev  

	

