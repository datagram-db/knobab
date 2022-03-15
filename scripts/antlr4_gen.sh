#!/bin/bash
java -jar ../submodules/antlr4/tool/target/antlr4-4.9.3-SNAPSHOT-complete.jar -Dlanguage=Cpp -visitor $1 #-o antlr4/cpp