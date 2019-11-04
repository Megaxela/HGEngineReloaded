#!/bin/bash

script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
. "${script_dir}/tools.sh"
. "${script_dir}/variables.sh"

function collect_coverage_info() {
  lcov --gcov-tool /usr/bin/gcov-8 --compat-libtool --directory "$build_dir" --base-directory "$source_dir" --capture --output-file "$build_dir/coverage.info"
}

function remove_coverage_info_for() {

  for glob in "$@"; do
    if ! lcov --gcov-tool /usr/bin/gcov-8 --compat-libtool --remove "$build_dir/coverage.info" $glob -o "$build_dir/coverage.info"; then
      echo "Can't remove coverage info for '$glob'."
      return $FALSE
    fi
  done

  return $TRUE
}

function perform_coverage_check() {
  ignore_directories=(
    '*/tests/*'
    '*/testing/*'
    '*/libraries/*'
    '*/gcc/*'
    '/usr/include/*'
  )

  if ! (
    # Collecting coverage info
    if ! collect_coverage_info; then
      >&2 echo "Can't collect coverage info."
      return $FALSE
    fi

    if ! remove_coverage_info_for $ignore_directories; then
      >&2 echo "Can't remove coverage info."
      return $FALSE
    fi

    coveralls-lcov "$build_dir/coverage.info"

    return $TRUE
  ); then
    return $FALSE
  fi
  return $TRUE
}