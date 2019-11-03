#!/bin/bash

script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
. "${script_dir}/tools.sh"

function perform_test() {
  if ! (
    cd "$build_dir/testing/"
    ./HGEngineTests
  ); then
    return $FALSE
  fi

  return $TRUE
}