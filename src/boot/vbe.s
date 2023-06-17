.code16
.section .data

.set vbe_info_signature, 0x32454256
.set vbe_info_version, 0x300
.set vbe_info_ptr_offset, 14
.set vbe_info_struct_offset, 0x500

.set mode_info_width_offset, 18
.set mode_info_height_offset, 20
.set mode_info_bpp_offset, 25
.set mode_info_buffer_offset, 40
.set mode_info_struct_offset, 0x800

.section .text
    .globl set_video_mode

set_video_mode:
    movw %ax, .width(,1)
    movw %bx, .height(,1)
    movb %cl, .bpp(,1)
    
    movl $0, %eax
    movw $1024, %cx
.clear_loop:
    movw %cx, %di
    movb %al, vbe_info_struct_offset - 1(%di)
    loop .clear_loop
    
    movl $vbe_info_signature, %eax
    movl %eax, vbe_info_struct_offset(,1)
    movw $vbe_info_version, %ax
    movw %ax, vbe_info_struct_offset+4(,1)
    
    push %es
    movw $0x4F00, %ax
    movw $vbe_info_struct_offset, %di
    int $0x10
    pop %es
    
    cmp $0x4F, %ax
    jne .error
    
    movw vbe_info_ptr_offset(%di), %ax
    movw %ax, .offset(,1)
    movw vbe_info_ptr_offset+2(%di), %ax
    movw %ax, .segment(,1)
    
    movw %ax, %fs
    movw .offset(,1), %si
    
.find_mode:
    movw %fs:(%si), %dx
    add $2, %si
    movw %si, .offset(,1)
    movw %dx, .mode(,1)
    movw $0, %ax
    movw %ax, %fs
    
    cmp $0xFFFF, .mode(,1)
    jne .skip
    movw $0x4315, .mode(,1)
.skip: 
    push %es
    movw $0x4F01, %ax
    movw .mode(,1), %cx
    movw $mode_info_struct_offset, %di
    int $0x10
    pop %es
    
    movw .width(,1), %ax
    cmp mode_info_struct_offset+mode_info_width_offset(,1), %ax
    jne .next_mode
    
    movw .height(,1), %ax
    cmp mode_info_struct_offset+mode_info_height_offset(,1), %ax
    jne .next_mode
    
    movb .bpp(,1), %al
    cmp mode_info_struct_offset+mode_info_bpp_offset(,1), %al
    jne .next_mode
    
    # found
    
    push %es
    movw $0x4F02, %ax
    movw .mode(,1), %bx
    or $0x4000, %bx
    int $0x10
    pop %es
    
    movl mode_info_struct_offset+mode_info_buffer_offset(, 1), %eax
    ret

.next_mode:
    movw .segment(,1), %ax
    movw %ax, %fs
    movw .offset(,1), %si
    jmp .find_mode
    
    
.error:
    hlt
    jmp .error


.section .data

.width: .word 0
.height: .word 0
.bpp: .byte 0
.offset: .word 0
.segment: .word 0
.mode: .word 0
