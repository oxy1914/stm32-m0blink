/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) 2025 - Aleksandr Loktionov */

/* Memories definition */
MEMORY
{
  RAM (xrw)		: ORIGIN = 0x20000000, LENGTH = 4K
  ROM (rx)		: ORIGIN = 0x8000000, LENGTH = 16K
}

/* Highest address of the user mode stack */
_estack = ORIGIN(RAM) + LENGTH(RAM);	/* end of RAM */

/* Sections */
SECTIONS
{
  /* The program code and other data into ROM memory */
  .text :
  {
    . = ALIGN(4);
    *(.vectors)
    KEEP(*(.vectors))
    *(.text)           /* .text sections (code) */
  } >ROM

  /* Constant data into ROM memory*/
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
  } >ROM

  .sdata :{
    . = ALIGN(4);
    _sdata = .;
  } >RAM

  /* Initialized data sections into RAM memory */
  .sidata :{
    . = ALIGN(4);
    _sidata = .;
  } >ROM

  .data :{
    . = ALIGN(4);
    *(.data .data.*)
    *(.sdata .sdata.*)
    . = ALIGN(4);
    _edata = .;
  } >RAM AT>ROM

  /* Uninitialized data section into RAM memory */
  .bss :{
    . = ALIGN(4);
    _sbss = .;
    *(.sbss*)
    *(.bss*)
    . = ALIGN(4);
    _ebss = .;
  } >RAM
}
