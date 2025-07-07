#!/bin/bash

rm -rf build

cmake \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
  -DCMAKE_CXX_COMPILER=$INSTP/bin/clang++ \
  -DCMAKE_OSX_SYSROOT="macosx" \
  -DCMAKE_PREFIX_PATH=$INSTP \
  -DCMAKE_CXX_FLAGS="-nostdinc++ -stdlib=libc++ -fexperimental-library -isystem $INSTP/include/c++/v1" \
  -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=lld -L$INSTP/lib -Wl,-rpath,$INSTP/lib" \
  -S . -B build -G "Ninja" -Wno-dev
