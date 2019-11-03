#!/bin/bash

function install_packages() {
  sudo apt update
  sudo apt install -y "$1"
}

function add_repositories() {
  for arg in $1; do
    sudo add-apt-repository "$arg";
  done
}

function add_keys() {
  for arg in $1; do
    wget -O - "$arg" | sudo apt-key add -;
  done
}