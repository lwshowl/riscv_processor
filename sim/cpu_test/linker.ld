ENTRY(_start)

SECTIONS {
  . = 0x80000000;
  .text  : {
    *(entry)
    *(.text*)
  }
  etext = .;
  _etext = .;
  .rodata 0x81000000 : {
    *(.rodata*)
  }
  .data 0x82000000: {
    *(.data)
  }
  edata = .;
  _data = .;
  .bss : {
	_bss_start = .;
    *(.bss*)
    *(.sbss*)
    *(.scommon)
  }

  _stack_top = ALIGN(0x1000);
  . = _stack_top + 0x8000;
  _stack_pointer = .;
  end = .;
  _end = .;
  _heap_start = ALIGN(0x1000);
}
