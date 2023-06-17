@echo off

IF "%1" == "" (
    wsl -e sh build.sh
) ELSE (
    wsl -e sh build.sh %1
)

pause