.code16

.include "src/boot/gdt.s"

.set _temp_core_location_segment, 0x7E0
.set _temp_core_location_offset, 0x0

.set _video_width, 800
.set _video_height, 600
.set _video_bpp, 32

.extern _core_begin
.extern _core_size_int
.extern _core_size_sectors

.extern set_video_mode

.section .data
    .globl video_buffer
    video_buffer: .int 0x0

.align 4
    read_core_packet:
    .byte 0x10
    .byte 0x0
    .short _core_size_sectors
    .short _temp_core_location_offset
    .short _temp_core_location_segment
    .int 0x01
    .int 0x00
    

.section .text
    .globl _start

_start:
    movw $0x7000, %ax
    movw %ax, %ss
    movw $0xFFFF, %ax
    movw %ax, %sp

    mov $0x41, %ah
    mov $0x55AA, %bx
    int $0x13
    jnc _test_passed
_test_failed:
    hlt
    jmp _test_failed

_test_passed:

    xor %ax, %ax
    mov %ax, %ds
    mov $read_core_packet, %ax
    mov %ax, %si
    mov $0x42, %ah
    
    int $0x13

    xor %ax, %ax
    mov %ax, %es
    
    movw $0x2401, %ax
    int $0x15
    
    movw $_video_width, %ax
    movw $_video_height, %bx
    movb $_video_bpp, %cl
    call set_video_mode
    
    movl %eax, video_buffer(,1)
    
    cli
    
    lgdt gdt_ptr
    movl %cr0, %eax
    or $1, %eax
    movl %eax, %cr0
    
    jmp $0x8,$second_part

.code32
second_part:
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %gs
    movw %ax, %fs
    movw %ax, %ss
    
    movl $0x7FFFF, %eax
    movl %eax, %esp
    movl %eax, %ebp
    
    movl $_core_size_int, %ecx
    movl $_core_begin, %edi
    movl $_temp_core_location_segment*0x10+_temp_core_location_offset, %esi
    
    rep movsd
    
    push video_buffer
    jmp _core_begin
