#!/bin/bash

script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
. "${script_dir}/tools.sh"

function prepare_compiler() {
  local compiler="$1"

  export ZLIB_PREFIX="/usr/local/"
  case $compiler in
    clang)
      export CC=`which clang-10`
      export CXX=`which clang++-10`
      ;;
    gcc)
      export CC=`which gcc`
      export CXX=`which g++`
      ;;
    mingw-w64)
      export CC=`which x86_64-w64-mingw32-gcc-posix`
      export CXX=`which x86_64-w64-mingw32-g++-posix`
      export AR=`which x86_64-w64-mingw32-ar`
      export RUNLIB=`which x86_64-w64-mingw32-ranlib`
      export ZLIB_PREFIX="/usr/x86_64-w64-mingw32"
      export CMAKE_ADDITIONAL_CONFIGURE_ARGS=(
          "-DCMAKE_SYSTEM_NAME=Windows"
          "-DCMAKE_FIND_ROOT_PATH=/usr/x86_64-w64-mingw32"
          "-DCMAKE_RC_COMPILER=$(which x86_64-w64-mingw32-windres)"
          "-DDLLTOOL=$(which x86_64-w64-mingw32-dlltool)"
          "-DCMAKE_INSTALL_PREFIX=/usr/x86_64-w64-mingw32"
          "-DWIN32=ON"
          "-DMINGW=ON"
      )
      ;;
    *)
      >&2 echo "Unknown compiler $compiler"
      return $FALSE
      ;;
  esac

  return $TRUE
}