#!/bin/bash

script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
. "${script_dir}/packages.sh"

function install_common_dependencies () {
  local packages=(
  # Tools
    clang-format-9

  # Compilation
    build-essential

  # Graphics
    xorg-dev
    libx11-dev
    libxmu-dev
    libxi-dev
    libgl1-mesa-dev
    libglu1-mesa-dev
    libxxf86vm-dev
    libxrandr-dev
    libxinerama-dev
    libxcursor-dev

  # Dependencies
    libglu-dev
    libgl-dev
  )
  install_packages "${packages[@]}"
}