#!/bin/bash

function install_packages() {
  sudo apt update
  sudo apt install -y "$@"
}

function add_repositories() {
  for arg in "$@"; do
    echo "Adding key '$arg'"
    yes | sudo add-apt-repository "$arg";
  done
}

function add_keys() {
  for arg in "$@"; do
    echo "Adding key '$arg'"
    wget -O - "$arg" | sudo apt-key add -;
  done
}