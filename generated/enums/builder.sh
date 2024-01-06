#!/bin/bash

clean(){
    rm *.cpp
    rm *.hpp
    rm enum_parser
}

first_jounger(){
    if [[ $1 -nt $2 ]] then echo true; else echo false; fi
}

exist(){
    if [[ -f $1 ]] then echo true; else echo false; fi
}

compile_enum(){
    target=${1%%.en}
    cat sources/$1 | ./enum_reader $target
    gcc -c $target.cpp -o obj/$target.o
}

build(){
    touch .tmp
    cd parser && make
    cd ..

    if [[ parser/enum_reader -nt .tmp ]]; then
        cp parser/enum_reader .
        for file in $(ls sources); do
            compile_enum $file
        done

    else
        for file in $(ls sources); do
            target=${file%%.en}.hpp
            if ( ! $(exist $target) || $(first_jounger sources/$file $target) ); then
                compile_enum $file
            fi
        done
    fi

    rm .tmp
}

case $1 in
    "build")
        build
        ;;
    "clean")
        clean
        ;;
    *)
        echo not supported action
esac