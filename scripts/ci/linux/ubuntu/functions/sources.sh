#!/bin/bash

script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
. "${script_dir}/packages.sh"

function install_sources () {
  keys_from=(
    'https://apt.kitware.com/keys/kitware-archive-latest.asc'
  )

  sources=(
    'deb https://apt.kitware.com/ubuntu/ focal main'
  )

  add_keys "${keys_from[@]}"
  add_repositories "${sources[@]}"
}