#!/bin/bash

script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
. "${script_dir}/variables.sh"
. "${script_dir}/tools.sh"

function perform_build () {
  if ! cmake "${CMAKE_ADDITIONAL_CONFIGURE_ARGS[@]}" $@ "-B$build_dir" "-H$source_dir"; then
    >&2 echo "Can't configure project."
    return $FALSE
  fi

  if ! cmake --build "$build_dir" --target all -- -j4; then
    >&2 echo "Can't build project."
    return $FALSE
  fi

  return $TRUE
}