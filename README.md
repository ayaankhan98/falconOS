# falcon OS - Minor Project Lab

![Alt text here](preview_shots/falconOS.svg)

----

falcon OS is a basic operating system from scratch aiming to follow Monolithic Kernel Architecture. It's a kind of hobby Operating System created out of interest in order to explore the fields of Kernel Development, System Programming, Thread safe Data Structures and Algorithms Design, Large Scale Software Design.

## Features - so far what we have?
- [x] Segmentation based Memory architecture
- [x] PIC
- [x] Basic Device Drivers for Mouse and Keyboard
- [x] Basic Event System for Mouse and Keyboard
- [x] Interrupt and Exception Management
- [x] Device Management
- [x] Dynamic Memory Allocation
- [x] PCI
- [x] Basic System Calls
- [x] Basic GUI support of 320 x 200 with 8-bit color depth channel
- [x] Multitasking

## Development Tools and Tech Stack
- Intel x86 Assembly
- C++ Toolchain
- GNU Make
- NASM and GAS Assembler
- Xorriso for ISO image building
- LLVM tools clang-format, clang-tidy
- QEMU or Oracle Virtual Box or Bosch
- Development Environment: Linux (Preffered)

## Building

### Arch Linux

Install GCC compiler
```
sudo pacman -Sy gcc
```
Install xorriso for ISO building
```
sudo pacman -Sy xorriso
```
Install mtools
```
sudo pacman -Sy mtools
```
Install NASM Assembler
```
sudo pacman -Sy nasm
```
Install Grub 
```
sudo pacman -Sy grub
```
use `Makefile` to build `kernel.iso` target
```
make kernel.iso
```
once you have ISO file, run it in VBox or QEMU.


### Ubuntu
install the same packages using `apt` package manager.

---

## Contributors
- [Ayaan Khan](https://github.com/ayaankhan98)
- [Hashir Hameed Niazi](https://github.com/HashirGJ8842)

## Development Status
In Progress...
