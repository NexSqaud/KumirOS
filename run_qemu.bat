@echo off

IF NOT EXIST "release.img" (
    build.bat release
)

qemu-system-i386 -s -name "KumirOS" -hda release.img
