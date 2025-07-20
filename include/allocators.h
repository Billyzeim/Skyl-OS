#ifndef ALLOCATORS_H
#define ALLOCATORS_H

void* alloc_page();
void free_page(void* addr);
void init_pmm();

#endif