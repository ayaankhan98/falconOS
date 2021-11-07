ENTRY(loader)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

SECTIONS
{
  . = 0x0100000;

  .text :
  {
    *(.multiboot)
    *(.text*)
    *(.rodata)
    . = ALIGN(4096);
  }

  .data :
  {
    start_ctors = .;
    KEEP(*(_init_array));
    KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)));
    end_ctors = .;
    
    *(.data)
    . = ALIGN(4096);
  }

  .bss :
  {
    *(.bss)
    . = ALIGN(4096);
  }

  end = .; _end = .; __end = .;

  /DISCARD/ :
  {
    *(.fini_array*)
    *(.comment)
  }
}
