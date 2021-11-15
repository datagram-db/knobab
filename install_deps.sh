#!/usr/bin/env bash
# Installing the required packages
sudo apt-get install -y uuid-dev maven libboost1.71-all-dev librapidxml-dev

## Pulling the repositories
git submodule update --init --recursive
git submodule update --recursive --remote