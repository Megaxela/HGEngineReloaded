#!/bin/bash

script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
. "${script_dir}/variables.sh"
. "${script_dir}/tools.sh"

function check_codestyle() {
  if ! "${source_dir}/scripts/check_relative_includes.sh"; then
    return $FALSE
  fi

  "${source_dir}/scripts/run_clang_format.sh"

  if ! check_nothing_changed; then
    return $FALSE
  fi
  return $TRUE
}