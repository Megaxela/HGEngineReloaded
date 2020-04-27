#!/bin/bash

script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
. "${script_dir}/tools.sh"

function prepare_compiler() {
  local compiler="$1"

  case $compiler in
    clang)
      export CC="/usr/local/clang-7.0.0/bin/clang"
      export CXX="/usr/local/clang-7.0.0/bin/clang++"
      ;;
    gcc)
      export CC="gcc-8"
      export CXX="g++-8"
      ;;
    mingw-w64)
      echo "DEBUG!! : " `which x86_64-w64-mingw32-windres`
      echo "DEBUG!! : " `which x86_64-w64-mingw32-dlltool`
      export CC="/usr/bin/x86_64-w64-mingw32-gcc-posix"
      export CXX="/usr/bin/x86_64-w64-mingw32-g++-posix"
      export CMAKE_ADDITIONAL_CONFIGURE_ARGS=(
          "-DCMAKE_SYSTEM_NAME=Windows"
          "-DCMAKE_FIND_ROOT_PATH=/usr/x86_64-w64-mingw32"
          "-DCMAKE_RC_COMPILER=/usr/bin/x86_64-w64-mingw32-windres"
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