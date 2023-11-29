#!/bin/bash
if [[ $1 == "aarch64" ]]; then
  echo "BUILD ARM-64"
  export CROSS_COMPILER=aarch64-linux-gnu-
  export CC=${CROSS_COMPILER}gcc
  export AR=${CROSS_COMPILER}ar
  export CFLAGS=""
  export ARCH="aarch64"
elif [[ $1 == "armv7" ]]; then
  echo "BUILD ARMV7"
  export CROSS_COMPILER=arm-linux-gnueabihf-
  export CC=${CROSS_COMPILER}gcc
  export AR=${CROSS_COMPILER}ar
  export CFLAGS=""
  export ARCH="armv7"
elif [[ $1 == "i386" ]]; then
  echo "BUILD i386"
  export CC=gcc
  export AR=ar
  export CFLAGS="-m32"
  export ARCH="i386"
else
  echo "BUILD x86-64"
  export CC=gcc
  export AR=ar
  export CFLAGS=""
  export ARCH="amd64"
fi

mkdir -p bin
echo "BUILD EXAMPLE"
${CC} -O2 $CFLAGS \
  example/libjmentrance-test.c \
  lib/libjmentrance-$ARCH.a \
  -Iinclude \
  -o bin/libjmentrance-test-$ARCH

echo "FINISH...";

