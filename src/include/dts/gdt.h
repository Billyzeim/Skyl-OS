#ifndef GDT_H
#define GDT_H

void init_gdt();
void load_gdt(void *gdt_ptr);

#endif