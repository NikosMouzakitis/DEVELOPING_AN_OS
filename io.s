global outb
global inb
global ld_gdt

; outb sends a byte to an io port.
;	stack : [esp+8] databyte
;		[esp+4] the ioport
;		[esp  ] return address

outb:	
	mov	al, [esp + 8] ; al <- data to be send.
	mov	dx, [esp + 4] ; dx <- address to be send.
	out	dx, al	      ; send to dx value of al.
	ret		      ; return to caller.

; inb returns a byte from a given i/o port.
; stack:	[esp+4] :address of i/o port
;		[esp]	:return address

inb:
	mov	dx, [esp+4]	;move the address of the ioport to ->dx
	in	al, dx		;read byte from ioport and store it ->al	
	ret

; ld_gdt loads the Global Descriptor Table.
; Address of the struct gdt is supposed to be in eax.

ld_gdt:
	lgdt	[eax]		
	;code here uses the previous cs
	jmp	0x08: flush_cs
flush_cs:
	;now cs is changed to 0x08.
	mov	ax, 0x20	
	mov	ds, ax
	mov	ss, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax
	ret
