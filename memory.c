
#include "multiboot.h"
#include "x86.h"
#include "framebuffer.h"
#include "memory.h"
#include "strlib.h"

#define CEIL_DIV(a,b) (((a+b)-1)/b)

//frame is a physical representation of a page.
static u32 pageFrameMin;
static u32 pageFrameMax;
static totalAlloc; //how many pages allocated? 
#define NUM_PAGES_DIRS 256  //we ll use 256 page directories.
#define NUM_PAGE_FRAMES (0x10000000/0x1000/8)

u8 physicalMemoryBitmap[NUM_PAGE_FRAMES/8]; //should be done like a bitArray dynamic.
static u32 pageDirs[NUM_PAGES_DIRS][1024] __attribute__((aligned(4096))); 
static u8 pageDirUsed[NUM_PAGES_DIRS]; //used ones

//physical memory management
void pmm_init(u32 memLow, u32 memHigh)
{
	pageFrameMin = CEIL_DIV(memLow, 0x1000); ///0x1000 = 4096 
	pageFrameMax = memHigh/0x1000; ///0x1000 = 4096 
//	printf("paFMin: %x",pageFrameMin);
//	pprint("");
//	printf("paFMax: %x",pageFrameMax);
//	pprint("");
	totalAlloc = 0;
	memset(physicalMemoryBitmap, 0, sizeof(physicalMemoryBitmap));
}
void init_memory(u32 memHigh, u32 physicalAllocStart)
{

	//invalidating the initial 4KB mapping
	initial_page_dir[0] = 0;
	invalidate(0);

	//recursive mapping from the end.
	initial_page_dir[1023] = ( (u32) initial_page_dir - KERNEL_START) | PAGE_FLAG_PRESENT | PAGE_FLAG_WRITE;
	invalidate(0xFFFFF000);
	printf("iniPDIR: %x",initial_page_dir);
	pprint("");
//	printf("&iPD[1]: %x",&initial_page_dir[1]);
//	pprint("");
	
	//set the second entry of the page directory with present and write flags.
	//address : 0x0  flags: present and writable.
	initial_page_dir[1] = 0x3;   

//	pmm_init(physicalAllocStart, memHigh);
	//zero out all structure holding page Directories.
//	memset(pageDirs, 0, 0x1000*NUM_PAGES_DIRS);
	//zero out all structure holding page directories used.
//	memset(pageDirUsed, 0,NUM_PAGES_DIRS);
}

void invalidate(u32 vaddr)
{
	asm volatile("invlpg %0" :: "m"(vaddr));
}
