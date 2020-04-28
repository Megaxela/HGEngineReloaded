#!/bin/bash

script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
. "${script_dir}/packages.sh"
. "${script_dir}/tools.sh"

function install_lcov_from_sources() {
  local path="$1"

  # Getting lcov
  if ! git clone https://github.com/linux-test-project/lcov.git "$path/lcov"; then
    >&2 echo "Can't download lcov."
    return $FALSE
  fi

  if ! $SUDO_EXEC make -C "$path/lcov" install; then
    >&2 echo "Can't compile and install lcov."
    return $FALSE
  fi

  return $TRUE
}

function install_coverage_dependencies() {
  local external_path="$1"

  # Declarating packages
  local packages=(
  # Coverage
    ggcov

  # Dependencies
    libperlio-gzip-perl
    libjson-perl
  )

  if ! install_packages "${packages[@]}"; then
    >&2 echo "Can't install packages."
    return $FALSE
  fi

  if ! install_lcov_from_sources "$external_path"; then
    >&2 echo "Can't install lcov from sources."
    return $FALSE
  fi
  if ! gem install coveralls-lcov; then
    >&2 echo "Can't install coveralls lcov."
    return $FALSE
  fi

  return $TRUE
}