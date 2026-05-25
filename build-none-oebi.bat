@ECHO OFF
REM
REM TOOLCHAIN
REM
set CC=glibc-oabi-toolchain-arm-generic\bin\arm-unknown-linux-gnu-gcc.exe
set AR=D:\devtools\glibc-oabi-toolchain-arm-generic\bin\arm-unknown-linux-gnu-ar.exe
@REM set PATH=%PATH%;D:\_tol\artos\devices\libjjs\lib\obj\glibc-oabi-toolchain-arm-generic\bin

@REM echo %PATH%
 
%CC% ^
  example/libjmentrance-test.c ^
  lib/libjmentrance-noneabi-arm.a ^
  -Iinclude ^
  -o bin/libjmentrance-test-noneabi

