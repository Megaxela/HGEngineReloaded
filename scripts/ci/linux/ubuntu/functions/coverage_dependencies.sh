#!/bin/bash

script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
. "${script_dir}/packages.sh"

function install_coverage_dependencies() {
  # Declarating packages
  local packages=(
  # Coverage
    lcov
    ggcov

  # Dependencies
    libperlio-gzip-perl
    libjson-perl
  )

  install_packages "${packages[@]}"
}