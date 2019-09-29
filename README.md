Authors:- Maharsh Suryawala, Nikhil Balwani, Parshwa Shah, Jay Patel

# xv6-PDOS-bootloader #

This is a re-implementation of xv6-PDOS Operating System's Bootloader in C. The complete x86-assembly and C implementation of this Operating System can be found [here](https://github.com/mit-pdos/xv6-public).

## Salient features ##
* Data structure for ELF (file format) Header.
* Data structure for Program Header.
* Function to access a sector with a given offset from the given address.
* Blocking call to wait for the disk to be ready before any file operations can be performed.

## ELF Header ##

**Executable Linkable Format** (ELF) is the format in which Program Headers are stored. Whenever the power button is pressed, the contract with the hardware is such that it will bring Program Headers from *the Boot Sector* (first sector of the hard drive) to the memory, where the program headers will be executed.

An ELF header has the following important fields:

* **Magic Number**. To check the integrity of ELF header block, also verfies whether the Operating system is running in correct mode.
* **Program Header Offset** The number of bits to offset before we find the program headers
* **Number of Program Headers** Total number of program headers

## Program Header ##

It has the following important fields

* Type
* Offset
* Virtual Address
* Private Address
* File size of the Program Header
* Memory size of the Program Header
