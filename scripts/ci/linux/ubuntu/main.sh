#!/bin/bash

main_script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

. "${main_script_dir}/functions/variables.sh"
. "${main_script_dir}/functions/tools.sh"
. "${main_script_dir}/functions/prepare_compiler.sh"
. "${main_script_dir}/functions/common_dependencies.sh"
. "${main_script_dir}/functions/compiler_dependencies.sh"
. "${main_script_dir}/functions/external_dependencies.sh"
. "${main_script_dir}/functions/coverage_dependencies.sh"
. "${main_script_dir}/functions/script_dependencies.sh"
. "${main_script_dir}/functions/sources.sh"
. "${main_script_dir}/functions/codestyle.sh"
. "${main_script_dir}/functions/build.sh"
. "${main_script_dir}/functions/test.sh"
. "${main_script_dir}/functions/coverage.sh"

if [ -z "${HG_COMPILER_TOOL}" ]; then
  HG_COMPILER_TOOL=clang
fi

if [ -z "${EXTERNAL_DEPS_DIR}" ]; then
  EXTERNAL_DEPS_DIR="external-deps"
fi

if [ -z "${HG_COVERAGE}" ]; then
  HG_COVERAGE=OFF
fi

if [ -z "${HG_EXECUTE_TESTS}" ]; then
  HG_EXECUTE_TESTS=ON
fi

if [[ "${HG_COVERAGE}" -eq "ON" ]]; then
  HG_PROJECT_FLAGS="$HG_PROJECT_FLAGS -DHG_TEST_HG_COVERAGE=On"
fi

SUDO_EXEC=sudo
if [[ $EUID -eq 0 ]]; then
  echo "Run as root. Don't use sudo"
  SUDO_EXEC=''
fi

# JFF
mkdir --parent "${EXTERNAL_DEPS_DIR}"

function action_help() {
  echo
  echo "Usage: $0"
  echo
  echo "It's main CI script. You can modify execution flow"
  echo "by following setup arguments and environment variables."
  echo
  echo "Variables:"
  echo "    HG_COMPILER_TOOL     Specifies compiler. "
  echo "        clang            ('gcc' by default)"
  echo "        gcc"
  echo "        mingw-w64"
  echo
  echo "    HG_EXTERNAL_DEPS_DIR Directory for storing external dependencies."
  echo "                         ('external-deps' by default)"
  echo
  echo "    HG_PROJECT_FLAGS     Project configure (cmake) flags"
  echo
  echo "    HG_HG_COVERAGE          Enable coverage for this build"
  echo "        ON               ('OFF' by default)"
  echo "        OFF"
  echo
  echo "    HG_HG_EXECUTE_TESTS     Enable tests execution"
  echo "        ON               ('ON' by default)"
  echo "        OFF"
  echo
  echo "Actions:"
  echo "    -h, --help                   Display usage instructions"
  echo "        --dependencies           Install required dependencies for build"
  echo "        --check_codestyle        Check codestyle (this method will modify code)"
  echo "        --external_dependencies  Install external dependencies (glew for example)"
  echo "        --build                  Perform project build"
  echo "        --test                   Run tests"

  # shellcheck disable=SC2086
  return $QUITTED
}

function action_install_dependencies() {
  echo "Installing internal dependencies..."
  if ! install_script_dependencies; then
    echo "Can't install internal dependencies."
    exit 1
  fi

  echo "Install sources..."
  install_sources

  echo "Installing dependencies..."
  if ! install_common_dependencies; then
    echo "Can't install common dependencies."
    exit 1
  fi

  if [[ "${HG_COVERAGE}" -eq "ON" ]]; then
    if ! install_coverage_dependencies "${EXTERNAL_DEPS_DIR}"; then
      echo "Can't install coverage dependencies."
      exit 1
    fi
  fi

  if ! install_compiler_dependencies "${HG_COMPILER_TOOL}"; then
    echo "Can't install compiler dependencies."
    exit 1
  fi

  if ! prepare_compiler "${HG_COMPILER_TOOL}"; then
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

  if ! install_external_dependencies "${EXTERNAL_DEPS_DIR}"; then
    echo "Can't install external dependencies."
    exit 1
  fi

  return $EXECUTED
}

function action_build() {
  echo "Building..."
  if ! perform_build ${HG_PROJECT_FLAGS}; then
    echo "Build failed."
    exit 1
  fi

  return $EXECUTED
}

function action_test() {
  echo "Running tests"
  if ! perform_test; then
    echo "Tests failed."
    exit 1
  fi

  return $EXECUTED
}

function action_coverage() {
  echo "Checking coverage"
  if ! perform_coverage_check; then
    echo "Can't perform coverage check."
    exit 1
  fi

  return $EXECUTED
}

function perform_run() {
  action_install_dependencies
  action_check_codestyle
  action_install_external_dependencies
  action_build

  if [[ "${HG_EXECUTE_TESTS}" -eq "ON" ]]; then
    action_test
  fi

  if [[ "${HG_COVERAGE}" -eq "ON" ]]; then
    action_coverage
  fi
}

declare -A actions
actions=(
  ["--h"]=action_help
  ["--help"]=action_help
  ["--dependencies"]=action_install_dependencies
  ["--check_codestyle"]=action_check_codestyle
  ["--external_dependencies"]=action_install_external_dependencies
  ["--build"]=action_build
  ["--test"]=action_test
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
      return 0
    elif [ $result -eq $EXECUTED ]; then
      continue
    fi

    echo "Unknown argument '$arg'. Skipping."
  done

  perform_run

  return 0
}

main "$@"