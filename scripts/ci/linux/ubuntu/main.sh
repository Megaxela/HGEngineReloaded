#!/bin/bash

main_script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

. "${main_script_dir}/functions/variables.sh"
. "${main_script_dir}/functions/tools.sh"
. "${main_script_dir}/functions/common_dependencies.sh"
. "${main_script_dir}/functions/compiler_dependencies.sh"
. "${main_script_dir}/functions/external_dependencies.sh"
. "${main_script_dir}/functions/coverage_dependencies.sh"
. "${main_script_dir}/functions/sources.sh"
. "${main_script_dir}/functions/build.sh"

if [ -z "${COMPILER_TOOL}" ]; then
  COMPILER_TOOL=clang
fi

if [ -z "${EXTERNAL_DEPS_DIR}" ]; then
  EXTERNAL_DEPS_DIR="external-deps"
fi

if [ -z "${COVERAGE}" ]; then
  COVERAGE=OFF
fi


function action_help() {
  echo
  echo "Usage: $0"
  echo
  echo "It's main CI script. You can modify execution flow"
  echo "by following setup arguments and environment variables."
  echo
  echo "Variables:"
  echo "    COMPILER_TOOL       Specifies compiler. "
  echo "        clang           ('gcc' by default)"
  echo "        gcc"
  echo
  echo "    EXTERNAL_DEPS_DIR   Directory for storing external dependencies."
  echo "                        ('external-deps' by default)"
  echo
  echo "    PROJECT_FLAGS       Project configure (cmake) flags"
  echo
  echo "    COVERAGE            Enable coverage for this build"
  echo "        ON              ('OFF' by default)"
  echo "        OFF"
  echo
  echo "Actions:"
  echo "    -h, --help    Display usage instructions"

  # shellcheck disable=SC2086
  return $QUITTED
}

function action_install_dependencies() {
  echo "Install sources..."
  install_sources

  echo "Installing dependencies..."
  if ! install_common_dependencies; then
    echo "Can't install common dependencies."
    exit 1
  fi

  if [[ "${COVERAGE}" -eq "ON" ]]; then
    if ! install_coverage_dependencies; then
      echo "Can't install coverage dependencies."
      exit 1
    fi
  fi

  if ! install_compiler_dependencies "${COMPILER_TOOL}"; then
    echo "Can't install compiler dependencies."
    exit 1
  fi

  if ! prepare_compiler "${COMPILER_TOOL}"; then
    echo "Can't prepare compiler."
    exit 1
  fi

  # shellcheck disable=SC2086
  return $EXECUTED
}

function action_check_codestyle() {
  echo "Checking codestyle..."
  if ! check_codestyle; then
    echo "Codestyle failed..."
    print_changes
    exit 1
  fi

  # shellcheck disable=SC2086
  return $EXECUTED
}

function action_install_external_dependencies() {
  echo "Installing external dependencies..."
  mkdir --parent "${EXTERNAL_DEPS_DIR}"

  if ! install_external_dependencies "${EXTERNAL_DEPS_DIR}"; then
    echo "Can't install external dependencies."
    exit 1
  fi

  return $EXECUTED
}

function action_build() {
  echo "Building..."
  mkdir --parent
  perform_build "${FLAGS}"

  return $EXECUTED
}

function perform_run() {
  action_install_dependencies
  action_check_codestyle
  action_install_external_dependencies
  action_build
}

declare -A actions
actions=(
  ["--h"]=action_help
  ["--help"]=action_help
  ["--dependencies"]=action_install_dependencies
  ["--check_codestyle"]=action_check_codestyle
  ["--external_dependencies"]=action_install_external_dependencies
  ["--build"]=action_build
)

function try_exec_as_action() {
  local action="$1"

  if exists $action in actions; then
    ${actions[$action]}
    return $?
  fi

  return $SKIPPED
}

function main() {
  for arg in "$@"; do
    try_exec_as_action $arg
    local result=$?

    if [ $result -eq $QUITTED ]; then
      exit 0
    elif [ $result -eq $EXECUTED ]; then
      continue
    fi

    echo "Unknown argument '$arg'. Skipping."
  done

  perform_run
}

main "$@"