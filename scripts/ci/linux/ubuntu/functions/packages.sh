#!/bin/bash

function install_packages() {
  $SUDO_EXEC apt update
  $SUDO_EXEC apt install -y "$@"
}

function add_repositories() {
  for arg in "$@"; do
    echo "Adding key '$arg'"
    yes | $SUDO_EXEC add-apt-repository "$arg";
  done
}

function add_keys() {
  for arg in "$@"; do
    echo "Adding key '$arg'"
    wget -O - "$arg" | $SUDO_EXEC apt-key add -;
  done
}