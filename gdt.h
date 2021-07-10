#ifndef __GDT_H
#define __GDT_H
#include "types.h"

struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));


struct gdt_ptr
{
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

extern void _gdt_flush();
extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
extern void gdt_install();

typedef struct gdt_entry gdt_entry_t;
typedef struct gdt_ptr _gp;

gdt_entry_t gdt[3];
_gp gp;





#endif
