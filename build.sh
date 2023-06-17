#!/bin/sh

set -xe

if [ ! -d "$HOME/opt/cross" ]; then
    echo "You should compile GCC cross-compiler with libgcc"
    echo "Check: https://wiki.osdev.org/GCC_Cross-Compiler"
    exit 1
fi

if ! [ -x "$(command -v kumir2-llvmc)" ]; then
    echo "Make sure you compiled Kumir2 with LLVM compiler"
    echo "Check: https://github.com/NexSqaud/Kumir2-llvm"
    exit 1
fi

export PATH="$HOME/opt/cross/bin:$PATH"
libgcc_path=$(i686-elf-gcc -print-libgcc-file-name)
libgcc_folder=$(echo $libgcc_path | sed "s/libgcc.a//")
export LIBGCC_PATH="$libgcc_folder"

if [ ! -d "build/" ]; then 
    mkdir build
fi

cc -ggdb -I nobuild/ nobuild.c -o build/nobuild.o

if [ $? -eq 0 ]; then
    clear
    ./build/nobuild.o $1
fi
