
#include "multiboot.h"
#include "x86.h"
#include "framebuffer.h"
#include "memory.h"

#define CEIL_DIV(a,b) (((a+b)-1)/b)

//frame is a physical representation of a page.
static u32 pageFrameMin;
static u32 pageFrameMax;
static totalAlloc; //how many pages allocated? 
#define NUM_PAGES_DIRS 256
#define NUM_PAGE_FRAMES (0x10000000/0x1000/8)


void memset(void *dest, char val, uint32_t count){
    char *temp = (char*) dest;
    for (; count != 0; count --){
        *temp++ = val;
    }

}


u8 physicalMemoryBitmap[NUM_PAGE_FRAMES/8]; //should be done like a bitArray dynamic.
static u32 pageDirs[NUM_PAGES_DIRS][1024] __attribute__((aligned(4096))); 
static u8 pageDirUsed[NUM_PAGES_DIRS]; //used ones
void pmm_init(u32 memLow, u32 memHigh)
{
	pageFrameMin = CEIL_DIV(memLow, 0x1000); ///0x1000 = 4096 
	pageFrameMax = memHigh/0x1000; ///0x1000 = 4096 
	totalAlloc = 0;
	memset(physicalMemoryBitmap, 0, sizeof(physicalMemoryBitmap));
}
void init_memory(u32 memHigh, u32 physicalAllocStart)
{

	initial_page_dir[0] = 0;
	invalidate(0);

	//recursive mapping from the end.
	initial_page_dir[1023] = ( (u32) initial_page_dir - KERNEL_START) | PAGE_FLAG_PRESENT | PAGE_FLAG_WRITE;
	invalidate(0xFFFFF000);
	initial_page_dir[1] = 0x3;
	pmm_init(physicalAllocStart, memHigh);
	memset(pageDirs, 0, 0x1000*NUM_PAGES_DIRS);
	memset(pageDirUsed, 0,NUM_PAGES_DIRS);
}


void invalidate(u32 vaddr)
{
	asm volatile("invlpg %0" :: "m"(vaddr));
}
