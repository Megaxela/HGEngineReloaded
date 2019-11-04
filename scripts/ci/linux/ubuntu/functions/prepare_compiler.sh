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
    *)
      >&2 echo "Unknown compiler $compiler"
      return $FALSE
      ;;
  esac

  return $TRUE
}