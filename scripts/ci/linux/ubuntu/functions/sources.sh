#!/bin/bash

script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
. "${script_dir}/packages.sh"

function install_sources () {
  keys_from=(
    'https://apt.llvm.org/llvm-snapshot.gpg.key'
    'https://apt.kitware.com/keys/kitware-archive-latest.asc'
  )

  sources=(
#    'ppa:ubuntu-toolchain-r/test'
    'deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-9 main'
    'deb https://apt.kitware.com/ubuntu/ bionic main'
  )

  add_keys "${keys_from[@]}"
  add_repositories "${sources[@]}"
}