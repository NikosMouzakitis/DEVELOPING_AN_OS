#!/bin/bash
qemu-system-x86_64 -kernel kernel.elf \
	-chardev stdio,id=seabios \
        -device isa-debugcon,iobase=0x3f8,chardev=seabios \

