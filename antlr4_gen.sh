#!/bin/bash
java -jar submodules/antlr4/tool/target/antlr4-4.9.3-SNAPSHOT-complete.jar -Dlanguage=Cpp -visitor -o antlr4/cpp $1