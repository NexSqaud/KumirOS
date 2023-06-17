@echo off

IF NOT EXIST "debug.img" (
    build.bat
)

qemu-system-i386 -s -S -name "[DEBUG] KumirOS" -hda debug.img -serial file:"debug.log"
