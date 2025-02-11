
extern u32 initial_page_dir[1024];
#define KERNEL_START 0xC0000000   //the address where the kernel is placed in the higher half approach
#define PAGE_FLAG_PRESENT (1 << 0)
#define PAGE_FLAG_WRITE (1 << 1)

void init_memory(u32 memHigh, u32 physicalAllocStart);
