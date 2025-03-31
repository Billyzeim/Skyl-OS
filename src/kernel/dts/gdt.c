#include "../../include/types/ints.h"
#include "../../include/dts/gdt.h"

struct GDTEntry {
    uint16_t limit_low;  
    uint16_t base_low;   
    uint8_t base_middle; 
    uint8_t access;      
    uint8_t granularity;
    uint8_t base_high;   
} __attribute__((packed));

struct GDTPointer {
    uint16_t limit;  
    uint32_t base;   
} __attribute__((packed));

struct GDTEntry gdt[3];  // Null, Code, and Data segments
struct GDTPointer gdtp;

void set_gdt_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[num].access = access;
}

void init_gdt() {
    gdtp.limit = (sizeof(struct GDTEntry) * 3) - 1;
    gdtp.base = (uint32_t)&gdt;

    set_gdt_entry(0, 0, 0, 0, 0);                // Null descriptor
    set_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    set_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment

    load_gdt(&gdtp);  // Call assembly function to load the GDT
}
