# DEVELOPING_AN_OS
Development of an OS. simple of course.

Update: 

Compile using "make os.iso"

Run on QEMU 
------------
a) as ELF using "elfrun.sh" 
b) using the iso incorporating GRUB via "run.sh"

an image from early boot prints.
![img](https://github.com/NikosMouzakitis/DEVELOPING_AN_OS/blob/main/testos.png)


# qemu monitor commands-can be used to verify what we implement

*info mem:* shows areas where we have set page tables

*info irq:* can show us the enabled interrupts f.e irq0-timer, irq1-keyboard
and the number of their occurance.



#resources

*Multiboot specification format example code multiboot.h*
https://www.gnu.org/software/grub/manual/multiboot/html_node/multiboot_002eh.html#multiboot_002eh
