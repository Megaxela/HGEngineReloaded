#!/bin/bash

script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
. "${script_dir}/variables.sh"

ON=0
OFF=1

TRUE=0
FALSE=1

EXECUTED=10
SKIPPED=11
QUITTED=12

function check_nothing_changed () {
  if [[ $(git diff -- "${source_dir}" ":(exclude)${source_dir}/scripts/*" | wc -l) -ne 0 ]]; then
    return $FALSE
  fi
  return $TRUE
}

function print_changes() {
  git diff -- "${source_dir}" ":(exclude)${source_dir}/scripts/*";
}