#!/bin/bash
mkdir ProM
pushd ProM
echo "Please download Java from https://download.oracle.com/otn/java/jdk/8u321-b07/df5ad55fdd604472a86a45a217032c7d/jdk-8u321-linux-x64.tar.gz"
if [ ! -d prom-6.11-all-platforms ]; then
     wget http://www.promtools.org/prom6/downloads/prom-6.11-all-platforms.tar.gz
     tar xvzf prom-6.11-all-platforms.tar.gz 
     rm prom-6.11-all-platforms.tar.gz
fi
popd
