#!/bin/bash

script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
. "${script_dir}/tools.sh"

function install_glew() {
  local path="$1"

  # Getting glew
  if ! wget https://github.com/nigels-com/glew/archive/glew-2.1.0.tar.gz -q -O "$path/glew.tar.gz"; then
    >&2 echo "Can't download glew."
    return $FALSE
  fi

  if ! tar -xvzf "$path/glew.tar.gz" -C "$path/"; then
    >&2 echo "Can't untar glew."
    return $FALSE
  fi

  # Building glew
  if ! make -C "$path/glew-glew-2.1.0/auto"; then
    >&2 echo "Can't generate glew functions."
    return $FALSE
  fi

  if ! cmake "-B$path/glew-glew-2.1.0/build_dir" "-H$path/glew-glew-2.1.0/build/cmake"; then
    >&2 echo "Can't generate glew project."
    return $FALSE
  fi

  if ! sudo cmake --build "$path/glew-glew-2.1.0/build_dir" --target install -- -j4; then
    >&2 echo "Can't compile and install glew."
    return $FALSE
  fi

  return $TRUE
}

function install_gtest() {
  local path="$1"

  # Getting gtest
  if ! git clone https://github.com/google/googletest.git "$path/gtest"; then
    >&2 echo "Can't download gtest."
    return $FALSE
  fi

  if ! cmake "-B$path/gtest/build_dir" "-H$path/gtest/"; then
    >&2 echo "Can't configure gtest."
    return $FALSE
  fi

  if ! sudo cmake --build "$path/gtest/build_dir" --target install -- -j4; then
    >&2 echo "Can't compile and install gtest."
    return $FALSE
  fi

  return $TRUE
}

function install_external_dependencies() {
  local path="$1"

  if ! install_glew $path; then
    >&2 echo "Can't install glew as dependency."
    return $FALSE
  fi

  if ! install_gtest $path; then
    >&2 echo "Can't install gtest as dependency."
    return $FALSE
  fi

  return $TRUE
}