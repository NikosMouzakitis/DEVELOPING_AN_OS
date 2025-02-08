
#include "multiboot.h"
#include "x86.h"
#include "framebuffer.h"

void init_memory(struct multiboot_info *bootInfo)
{
	for(int i = 0; i < bootInfo->mmap_length; i+=sizeof(struct multiboot_mmap_entry))
	{
		struct multiboot_mmap_entry * mmmt = (struct multiboot_mmap_entry*)(bootInfo->mmap_addr + i);

		printf("Low addr: %x | High addr: %x | LowLen: %x | HighLen: %x | Size: %x  | Type: %x", mmmt->addr_low, mmmt->addr_high,mmmt->len_low,mmmt->len_high,mmmt->size, mmmt->type);
		pprint("");
	}
}
