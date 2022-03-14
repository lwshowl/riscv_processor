OUTPUT_ARCH( "riscv" )

_STACK_SIZE = DEFINED(_STACK_SIZE) ? _STACK_SIZE : 0x01000;

/* Specify the default entry point to the program */

MEMORY {
  com : ORIGIN = 0x00000000, LENGTH = 0x00001000
  mem : ORIGIN = 0x00001000, LENGTH = 0x00001000
}

ENTRY(_start)

SECTIONS
{
  . = 0x00000100;
  .text.init : { *(.text.init) } > com
  . = ALIGN(0x1000);
  
  .text : { *(.text) }  > com
  . = ALIGN(0x1000);
  .data : { *(.data) } > mem
  .data.string : { *(.data.string)} > mem
  .bss : { *(.bss) } > mem

  .stack : {
    . = ALIGN(4);
    _stack_end = .;
    . += _STACK_SIZE;
    _stack = .;
    __stack = _stack;
  } > mem
  _stack_end = .;

  _end = .;
  
}

