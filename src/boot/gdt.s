.global gdt_table
.global gdt_ptr

.section .data
gdt_ptr:
    .short gdt_table_end - gdt_table - 1
    .int gdt_table

gdt_table:
    .int 0x0, 0x0
    
    # core code segment: Base = 0, Limit = 0xFFFFFF
    .short 0xFFFF
    .short 0x0
    .byte 0x0
    .byte 0b10011110
    #       PPLSECRA
    .byte (0b1100) << 4 | 0b1111
    #        GSLR          Limit
    .byte 0x0
    
    #code data segment: Base = 0, Limit = 0xFFFFFF
    .short 0xFFFF
    .short 0x0
    .byte 0x0
    .byte 0b10010010
    #       PPLSEDWA
    .byte (0b1100) << 4 | 0b1111
    #        GSLR          Limit
    .byte 0x0
gdt_table_end:
