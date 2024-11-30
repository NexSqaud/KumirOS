@echo off

gdb -ex "target remote :1234" -ex "symbol-file build/debug/kumiros.dbg" -ex "b _start" -ex "b _init" -ex "display/5i $eip" -ex "c"