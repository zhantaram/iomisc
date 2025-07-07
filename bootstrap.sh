#!/bin/bash

rm -rf build

cmake \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
  -DCMAKE_CXX_COMPILER=$INSTP/bin/clang++ \
  -DCMAKE_OSX_SYSROOT="macosx" \
  -DCMAKE_PREFIX_PATH=$INSTP \
  -DCMAKE_CXX_FLAGS="-nostdinc++ -stdlib=libc++ -fexperimental-library -isystem $INSTP/include/c++/v1" \
  -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=lld -L$INSTP/lib -Wl,-rpath,$INSTP/lib" \
  -DCMAKE_EXPERIMENTAL_CXX_IMPORT_STD="a9e1cf81-9932-4810-974b-6eccaf14e457" \
  -DCMAKE_CXX_MODULE_STD=1 \
  -S . -B build -G "Ninja" -Wno-dev
