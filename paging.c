#include "paging.h"

// Define the page directory and kernel page tables (identity and higher-half).
// They must be 4 KB–aligned.
u32 page_directory[NUM_ENTRIES] __attribute__((aligned(4096)));
u32 identity_page_table[NUM_ENTRIES] __attribute__((aligned(4096)));
u32 kernel_page_table[NUM_ENTRIES] __attribute__((aligned(4096)));

// Fill the identity page table to map the first MB (0x00000000 - 0x000FFFFF)
static void init_identity_page_table(void) {
    u32 phys_addr = 0x00000000; // Start mapping from 0x00000000
    for (u32 i = 0; i < 512; i++) {  // 256 * 4 KB = 1 MB
        identity_page_table[i] = phys_addr | 3; // Present (bit 0) | Writable (bit 1)
        phys_addr += 4096; // Move to next 4 KB page
    }
}

// Fill the kernel page table for the higher half (1 MB and above)
static void init_kernel_page_table(void) {
    u32 phys_addr = 0x100000; // Start mapping from 1 MB
    for (u32 i = 0; i < NUM_ENTRIES; i++) {
        kernel_page_table[i] = phys_addr | 3; // Present and writable
        phys_addr += 4096; // Move to next 4 KB page
    }
}

// Initialize the page directory
static void init_page_directory(void) {
    // Clear all entries
    for (u32 i = 0; i < NUM_ENTRIES; i++) {
        page_directory[i] = 0;
    }
    
    // Identity map the first MB (0x00000000 - 0x000FFFFF)
    page_directory[0] = ((u32)identity_page_table) | 3; // First entry maps the first MB

    // Map higher-half (0xC0000000 and above) to the kernel
    page_directory[768] = ((u32)kernel_page_table) | 3; // Maps virtual 0xC0000000+
}

// Enable paging
static inline void enable_paging(void) {
    asm volatile (
        "movl %0, %%eax\n\t"
        "movl %%eax, %%cr3\n\t"        // Load page directory into CR3
        "movl %%cr4, %%ebx\n\t"
        "orl  $0x10, %%ebx\n\t"        // Enable PSE (optional)
        "movl %%ebx, %%cr4\n\t"
        "movl %%cr0, %%ebx\n\t"
        "orl  $0x80000000, %%ebx\n\t"  // Enable paging
        "movl %%ebx, %%cr0\n\t"
        :
        : "r"(page_directory)
        : "eax", "ebx"
    );
}

// Initialize paging
void init_paging(void) {
    init_identity_page_table();
    init_kernel_page_table();
    init_page_directory();
    enable_paging();
}

