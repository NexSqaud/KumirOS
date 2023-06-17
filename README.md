#KumirOS
---------------------------------

Операционная система, программы для которой можно писать на [КуМире](https://github.com/NexSqaud/kumir2/tree/llvm-fix)

## Компиляция

Компиляция под Windows происходит из-под WSL

Для компиляции необходимо установить `gcc`, `clang-11`, `kumir-llvmc` и `libgcc`

`kumir-llvmc` можно найти [здесь](https://github.com/NexSqaud/kumir2/tree/llvm-fix) (необходимо скомпилировать самому)

`libgcc` можно найти [здесь](https://wiki.osdev.org/GCC_Cross-Compiler) (необходимо скомпилировать самому)

После установки всех зависимостей 

``` bash
git clone https://github.com/NexSqaud/KumirOS
cd KumirOS
./build.sh
```

## Запуск

Для запуска используется файл `run_qemu_debug.bat` (release версия не работает)
