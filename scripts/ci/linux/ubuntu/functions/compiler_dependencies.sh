#!/bin/bash

script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
. "${script_dir}/packages.sh"
. "${script_dir}/tools.sh"

function install_compiler_dependencies () {
  local compiler="$1"

  case $compiler in
    clang)
      local packages=(
      # Required STD lib
        g++-8
        gcc-8
      )
      ;;
    gcc)
      local packages=(
      # Compiler
        g++-8
        gcc-8
      )
      ;;
    *)
      >&2 echo "Unknown compiler '$compiler'."
      return $FALSE
      ;;
  esac

  install_packages "${packages[@]}"

  return $TRUE
}