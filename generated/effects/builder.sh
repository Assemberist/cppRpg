#!/bin/bash

exist(){
    if [[ -f $1 ]] then echo true; else echo false; fi
}

is_file_jounger(){
    if [[ $1 -nt $2 ]] then echo true; else echo false; fi
}

compile(){
    cat sources/$1 | parser/effect_reader > ${1%%.eff}.part
}

clean(){
    cd parser && make clean
    cd ..
    rm *.part
    rm effect_calc.cpp
}

assemble(){
    cat head.stdpart > effect_calc.cpp

    for file in $(ls *.part); do
        echo "case ${file%.part}:{" >> effect_calc.cpp
        cat $file >> effect_calc.cpp
        echo "} break;" >> effect_calc.cpp
    done

    cat end.stdpart >> effect_calc.cpp
}

build(){
    need_assemble=false

    touch .tmp
    cd parser && make
    cd ..
    
    #.tmp is second arg!!!
    #if parser changed
    if ( $(is_file_jounger parser/effect_reader .tmp) ); then
        #rebuild all files
        need_assemble=true
        for file in $(ls sources); do
            compile $file
        done

    #if parser not changed
    else
        #rebuild only changed/new files
        for file in $(ls sources); do
            part=${file%%.eff}.part
            if( ! $(exist $part) || $(is_file_jounger sources/$file $part) ); then
                need_assemble=true
                compile $file
            fi
        done

    fi

    if( $need_assemble ); then assemble; fi

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