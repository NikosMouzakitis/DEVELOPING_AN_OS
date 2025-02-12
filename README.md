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

*info tlb:* can show us the TLB and the respective flags set for every page

*info irq:* can show us the enabled interrupts f.e irq0-timer, irq1-keyboard
and the number of their occurance.

*in case of compiling our own custom qemu source*

```
./configure --target-list=i386-softmmu --enable-debug --disable-strip
make -j 4
```
#resources

*Multiboot specification format example code multiboot.h*

https://www.gnu.org/software/grub/manual/multiboot/html_node/multiboot_002eh.html#multiboot_002eh


# Paging


CR0 register

```
+--------------------------------------------------------------------------------+
| PG  | CD  | NW  | AM  | WP  | NE  | ET  | TS  | EM  | MP  | PE  |              |
| (31)| (30)| (29)| (18)| (16)| (5) | (4) | (3) | (2) | (1) | (0) |              |
+--------------------------------------------------------------------------------+
PG  - Paging Enable
CD  - Cache Disable
NW  - Not Write-through
AM  - Alignment Mask
WP  - Write Protect
NE  - Numeric Error
ET  - Extension Type
TS  - Task Switched
EM  - Emulation
MP  - Monitor Coprocessor
PE  - Protection Enable
```


CR4 register

```
+--------------------------------------------------------------------------------+
| VME  | PVI  | TSD  | DE  | PSE  | PAE  | MCE  | PGE  | PCE  | OSFXSR | OSXMMEXCPT |
| (0)  | (1)  | (2)  | (3) | (4)  | (5)  | (6)  | (7)  | (8)  |   (9)  |    (10)   |
+--------------------------------------------------------------------------------+
VME  - Virtual 8086 Mode Extensions
PVI  - Protected-Mode Virtual Interrupts
TSD  - Time Stamp Disable
DE   - Debugging Extensions
PSE  - Page Size Extensions
PAE  - Physical Address Extension
MCE  - Machine-Check Enable
PGE  - Page Global Enable
PCE  - Performance-Monitoring Counter Enable
OSFXSR  - OS Support for FXSAVE/FXRSTOR
OSXMMEXCPT - OS Support for Unmasked SIMD FP Exceptions
```


in our case where we have CR4=0x10, we have set PSE, and unset PAE on the CR4 register indicating 
32bit paging. With 32bit paging, only 32-bit linear addresses translation is supported.

With PSE enabled 32-bit paging can use both 4-KByte pages and 4-Mbyte pages.

*notes from Intel's System Prog.Guide* : 
The processor uses the upper portion of a linear address to identify a series of paging-structure entries. The last of
these entries identifies the physical address of the region to which the linear address translates (called the page
frame). The lower portion of the linear address (called the page offset) identifies the specific address within that
region to which the linear address translates. Each paging-structure entry contains a physical address, which is either the address of another paging structure or
the address of a page frame. In the first case, the entry is said to reference the other paging structure; in the
latter, the entry is said to map a page.

In our case:
```
Linear Address (32-bit)
------------------------------------------------
| 10-bit DIR  | 10-bit TABLE  | 12-bit OFFSET |
------------------------------------------------
      ↓               ↓                 ↓
+---------------+  +---------------+  +------------------+
| Page Dir (PDE)|->| Page Table (PTE)|->|  Physical Frame |
+---------------+  +---------------+  +------------------+
```

# References 

Intel 64 and IA-32 Architectures Software Developer's Manual Volume 3(3A,3B,3C & 3D): System Programming Guide
