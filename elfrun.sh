#!/bin/bash
qemu-system-i386 -kernel kernel.elf \
	-chardev stdio,id=characted_dev \
        -device isa-debugcon,iobase=0x3f8,chardev=characted_dev  -d int,cpu_reset

	

