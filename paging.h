#ifndef PAGING_H
#define PAGING_H


#define NUM_ENTRIES 1024

typedef unsigned int	u32;
typedef unsigned short	u16;
typedef unsigned char	u8;


// The page directory must be 4 KB–aligned.
extern u32 page_directory[NUM_ENTRIES] __attribute__((aligned(4096)));
// Identity paging for the first megabyte
extern u32 identity_page_table[NUM_ENTRIES] __attribute__((aligned(4096)));
// The kernel page table used for mapping the kernel region 
// for virtual addresses starting at 0xC0000000.
extern u32 kernel_page_table[NUM_ENTRIES] __attribute__((aligned(4096)));

// Initializes the paging structures and enables paging.
void init_paging(void);

#endif /* PAGING_H */

