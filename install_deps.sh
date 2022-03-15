#!/usr/bin/env bash
# Installing the required packages
sudo apt-get install -y uuid-dev maven libboost-python1.74-dev
pip install git+https://github.com/whitemech/flloat.git
pip install opyenxes==0.3.0

## Pulling the repositories
git submodule update --init --recursive
chmod +x generate_ltlf_patterns.py
./generate_ltlf_patterns
##git submodule update --recursive --remote