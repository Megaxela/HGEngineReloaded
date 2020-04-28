#!/bin/bash

script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
. "${script_dir}/packages.sh"
. "${script_dir}/tools.sh"

function install_script_dependencies() {
    export DEBIAN_FRONTEND=noninteractive
    local packages=(
        wget
        gnupg
        software-properties-common
        tzdata
    )

    install_packages "${packages[@]}"

    ln -fs /usr/share/zoneinfo/America/New_York /etc/localtime
    dpkg-reconfigure --frontend $DEBIAN_FRONTEND tzdata

    return $TRUE
}