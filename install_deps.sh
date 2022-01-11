#!/usr/bin/env bash
# Installing the required packages
sudo apt-get install -y uuid-dev maven libboost-python1.74-dev
pip install git+https://github.com/whitemech/flloat.git

## Pulling the repositories
git submodule update --init --recursive
##git submodule update --recursive --remote