#!/bin/bash
qemu-system-x86_64 -kernel kernel.elf \
	-chardev stdio,id=characted_dev \
        -device isa-debugcon,iobase=0x3f8,chardev=characted_dev \

