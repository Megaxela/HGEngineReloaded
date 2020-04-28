#!/bin/bash

script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
. "${script_dir}/packages.sh"
. "${script_dir}/tools.sh"

function install_script_dependencies() {
    local packages=(
        wget
    )

    install_packages "${packages[@]}"

    return $TRUE
}