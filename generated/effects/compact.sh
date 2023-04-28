#!/bin/bash

cat head.stdpart > effect_calc.cpp
files=`ls *.part`

for file in $files; do
    echo "case ${file%.part}:{" >> effect_calc.cpp
    cat $file >> effect_calc.cpp
    echo "} break;" >> effect_calc.cpp
done

cat end.stdpart >> effect_calc.cpp