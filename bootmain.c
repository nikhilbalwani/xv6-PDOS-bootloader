// Written by Nikhil Balwani (1641070)
// Bootloader C code
// Copies ELF header from the disk and using the information available in it, 
// copies the program header into physical address space.

// x86 utilities
#include "x86.h"

// Magic Number to check if the retrieved header is indeed an ELF header
#define ELF_MAGIC 0x464C457FU

// In x86 architecture, each sector on the hard disk is 512 bytes long
#define SECTORSIZE 512

// ELF header data structure
struct elfhdr {
    // Must be equal to the magic number ELF_MAGIC
    unsigned int magic;
    unsigned char elf[12];
    unsigned short type;
    unsigned short machine;
    unsigned int version;
    unsigned int entry;
    
    // Physical Header offset
    unsigned int phoff;
    unsigned int shoff;
    unsigned int flags;
    unsigned short ehsize;
    unsigned short phentsize;
    
    // Total number of physical headers
    unsigned short phnum;
    unsigned short shentsize;
    unsigned short shnum;
    unsigned short shstrndx;
};

void waitdisk();
void readsect(void *dst, unsigned int offset);

// Program header data structure
struct proghdr {
  unsigned int type;
  
  // Offset of the program header
  unsigned int offset;
  
  // Virtual address
  unsigned int va;
  
  // Private address
  unsigned int pa;
  
  // File size of the program header
  unsigned int filesz;
  
  // Memory size of the program header
  unsigned int memsz;
  
  unsigned int flags;
  unsigned int align;
};

// Function to read a segment from the disk
void read_segment(unsigned char* va, unsigned int count, unsigned int offset);

void bootmain() {
    
    // Points to the entry point of kernel in the assembly code 
    // (is a function pointer in C)
    void (*entry)(void);
    
    // Pointer to elf header
    struct elfhdr *elf;
    
    // Program header pointers: ph points to the current program header
    // eph points to the last program header in the code
    struct proghdr *ph, *eph;
    
    // Scratch space to load program headers
    elf = (struct elfhdr *) 0x10000;
    
    // Read the first page
    read_segment((unsigned char *) elf, SECTORSIZE * 8, 0); // 4096 bytes
    
    // Check for the correctness of ELF file
    // The bootmain function should never return but if it returns, there is a 
    // bug in the code
    if (elf -> magic != ELF_MAGIC)
        return;
        
    // Offsetting the location where the program header begins
    ph = (struct proghdr *) ( (unsigned char *) elf + elf -> phoff);
    
    // Storing last location of all the program headers
    eph = ph + elf -> phnum;
    
    // Iterating to the locations of all the program headers one after another
    while (ph < eph) {
        // Reading the memory segment for a program header at the given memory
        // size and offset in the program header (dereferenced using ph -> )
        readseg( (unsigned char *) (ph -> va & 0xFFFFFF), ph -> filesz, ph -> offset);
        
        // If the memory size is greater than file size
        if(ph->memsz > ph->filesz)
            // Initialize the remaining bytes to zero
            stosb(ph -> va + ph->filesz, 0, ph->memsz - ph->filesz);
        
        // Advancing to the next program header
        ph += 1;
    }
    
    // Entry point for the kernel code from the ELF header
    entry = (void(*) (void)) (elf -> entry & 0xFFFFFF);
    
    // Call entry
    entry();
}

void waitdisk()
{
  // First two bits should be '01' when disk is ready
  while( ( inb(0x1F7) & 0xC0 ) != 0x40);
}

// Read sector from disk
void readsect(void *dst, unsigned int offset) {
    waitdisk();
    
    // Format ----> outb(port number, offset)
    
    outb(0x1F2, 1);
    outb(0x1F3, offset);
    outb(0x1F4, offset >> 8);
    outb(0x1F5, offset >> 16);
    outb(0x1F6, (offset >> 24) | 0xE0);
    outb(0x1F7, 0x20);
    
    waitdisk();
    
    // Write the sector to dst
    insl(0x1F0, dst, SECTORSIZE / 4);
}

void read_segment(unsigned char* va, unsigned int count, unsigned int offset)
{
  unsigned char* eva;

  eva = va + count;

  va -= offset % SECTORSIZE;

  offset = (offset / SECTORSIZE) + 1;

  for(; va < eva; va += SECTORSIZE, offset++)
    readsect(va, offset);
}
