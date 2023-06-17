@echo off

gdb -ex "target remote :1234" -ex "b *0x7C00" -ex "b *0x100000" -ex "c"