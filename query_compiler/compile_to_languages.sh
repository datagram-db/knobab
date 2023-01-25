#!/bin/bash
rm -rf java
rm -rf cs
rm -rf python3
mkdir python3
pushd src
haxe --java ../java/ kinj/KINJ.hx
haxe --cs   ../cs/   kinj/KINJ.hx
haxe --python  ../python3/kinj.py   kinj/KINJ.hx
popd 
rm -rf kinj
mvn archetype:generate -DgroupId=uk.knobab -DartifactId=kinj -DarchetypeArtifactId=maven-archetype-quickstart -DarchetypeVersion=1.4 -DinteractiveMode=false
cp -r java/src kinj/src/main/java
pushd kinj
mvn test-compile
popd
rm -rf java
rm -rf cs/bin/*
echo '#!/usr/bin/env python3' | cat - python3/kinj.py > temp && mv temp python3/kinj.py
