#include <stdint.h>
#include <paging.h>

// note to self: when writting the chapter for paging (especially vmm) remember to point out all the parts that should confuse readers
// before explaining them so that they don't get discouraged when there are 10 new questions after one is solved.

#define PAGE_DIR_VIRTUAL ((uint32_t*)0xFFFFF000)
#define PAGE_TABLE_VIRTUAL(pd_idx) ((uint32_t*)(0xFFC00000 + (pd_idx * 0x1000)))

void vmm_map(uintptr_t virt, uintptr_t phys, uint32_t flags) {
    uint32_t pd_idx = virt >> 22;
    uint32_t pt_idx = (virt >> 12) & 0x3FF;

    // 1. Ensure the Page Table exists
    if (!(PAGE_DIR_VIRTUAL[pd_idx] & 0x01)) {
        // PT not present, get a frame from PMM
        uintptr_t new_pt_phys = (uintptr_t)find_available_page();
        
        // Link it in PD (Present + Writable + User/Supervisor, etc)
        PAGE_DIR_VIRTUAL[pd_idx] = new_pt_phys | flags | 0x01;

        // Use recursive mapping to zero the NEW Page Table
        // This is safe because we just mapped it in the PD
        uint32_t* pt_ptr = PAGE_TABLE_VIRTUAL(pd_idx);
        
        // We must flush the TLB for the PT address itself so the MMU 
        // recognizes the new PD entry we just wrote.
        asm volatile("invlpg (%0)" :: "r"(pt_ptr) : "memory");
        
        memset(pt_ptr, 0, 4096);
    }

    // 2. Map the actual page
    uint32_t* pt = PAGE_TABLE_VIRTUAL(pd_idx);
    pt[pt_idx] = phys | flags | 0x01;

    // 3. Flush TLB for the virtual address we just mapped
    asm volatile("invlpg (%0)" :: "r"(virt) : "memory");
}