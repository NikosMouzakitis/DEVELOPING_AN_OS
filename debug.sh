#!/bin/bash
qemu-system-i386 -kernel kernel.elf \
    -chardev stdio,id=characted_dev \
    -device isa-debugcon,iobase=0x3f8,chardev=characted_dev \
    -monitor telnet:127.0.0.1:4444,server,nowait
