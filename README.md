# xv6-PDOS-bootloader #

This is a re-implementation of xv6-PDOS Operating System's Bootloader in C. The code has structures for "ELF header" and "Program Header". 

## ELF header ##

### The ELF header has following important fields:
* **Magic Number**. To check the integrity of ELF header block, also verfies whether the Operating system is running in correct mode.
* **Programming Header Offset** (The number of bits to offset before we find the Physical Headers)
* **Number of Programming Headers** (Total number of physical headers)

