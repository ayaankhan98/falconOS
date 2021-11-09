# falcon OS - Minor Project Lab

![Alt text here](preview_shots/falconOS.svg)

----

falcon OS is a basic operating system in Monolithic Architecture.

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
Install Xorriso for ISO building
```
sudo pacman -Sy xorriso
```
once you have ISO file, run it in VBox or QEMU.

---

## Contributors
- [Ayaan Khan](https://github.com/ayaankhan98)
- [Hashir Hameed Niazi](https://github.com/HashirGJ8842)

## Development Status
In Progress...