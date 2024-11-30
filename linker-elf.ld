ENTRY(_start);

SECTIONS
{
	_core_begin = 0x100000;

	.bootloader_text 0x7C00 :
	{
		*bootloader.s.o(.text);
		*.s.o(.text);
	}
	
	.bootloader_data :
	{
		*bootloader.s.o(.data);
		*.s.o(.data);
	}
	
	.sig 0x7DFE :
	{
		SHORT(0xAA55);
	}
	
	
	.core_text _core_begin :
	{
		core_begin = .;
		*(.text*);
	}
	
	.core_data :
	{
		*(.data*);
        *(.rodata*);
        *(COMMON);
        *(.bss);
        *(.bss*);
        *(.eh_frame);
		core_end = .;
	}	
	
	_core_size = core_end - core_begin;
    _core_size_sectors = (_core_size / 512) + 1;
    _core_size_int = (_core_size / 4) + 1;
	
	. = core_end == . ? . + 1 : .;
    
    . = ALIGN(0x200);
    
    . = . - 1;
    .align :
    {
        BYTE(0x0); 
    } 
};