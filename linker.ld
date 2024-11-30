/*OUTPUT_FORMAT("binary");*/
ENTRY(_start);

SECTIONS
{
	. = 0x7C00;
	
    .bootloader_text : AT(0x7C00)
    {
        *bootloader.s.o(.text);
        *.s.o(.text);
    }
    
    .bootloader_data : AT(ADDR(.bootloader_text) + SIZEOF(.bootloader_text))
    {
        *bootloader.s.o(.data);
        *.s.o(.data);
    } 
	
    .sig 0x7DFE : AT(0x1FE)
    {
        SHORT(0xAA55);
    } 
	
	bootloader_end = .;

	. = 0x7E00;

    .core_text :
    {
        _text = .;
        *(.text*);
        _text_end = .;
    }
	
    .core_data :
    {
        _data = .;
        *(.data);
        *(.rodata*);
        *(COMMON);
        *(.bss);
        *(.bss*);
        *(.eh_frame);
        _data_end = .;
    } 
	
    _core_size = _data_end - _text;
    _core_size_sectors = (_core_size / 512) + 1;
    _core_size_int = (_core_size / 4) + 1;
    
    . = _data_end == . ? . + 1 : .;
    
    . = ALIGN(0x200);
    
    . = . - 1;
    .align :
    {
        BYTE(0x0); 
    } 
    
    /DISCARD/ :
    {
        *(.note*);
        *(.iplt*);
        *(.igot*);
        *(.rel*);
        *(.comment);
        *(.debug*);
		*(.interp);
		*(.eh_frame_hdr);
    }
}