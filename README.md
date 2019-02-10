# xv6-PDOS-bootloader #

This is a re-implementation of xv6-PDOS Operating System's Bootloader in C. The code has structures for "ELF header" and "Program Header". 

## ELF Header ##

**Executable Linkable Format** (ELF) is the format in which Program Headers are stored. Whenever the power button is pressed, the contract with the hardware is such that it will bring Program Headers from *the Boot Sector* (first sector of the hard drive) to the memory, where the program headers will be executed.

### An ELF header has the following important fields:###

* **Magic Number**. To check the integrity of ELF header block, also verfies whether the Operating system is running in correct mode.
* **Program Header Offset** The number of bits to offset before we find the Physical Headers
* **Number of Program Headers** Total number of program headers

## Program Header ##

### It has the following important fields ###

* Type
* Offset
* Virtual Address
* Private Address
* File size of the Program Header
* Memory size of the Program Header
