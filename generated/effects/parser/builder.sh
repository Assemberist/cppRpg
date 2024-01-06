#!/bin/bash

clean(){
	rm effect_reader lex.yy.c y.tab.h y.tab.c
}

is_file_jounger(){
    if [[ $1 -nt $2 ]] then echo true; else echo false; fi
}

files=(lexic.lex syntax.y effect_sup.c effect_sup.h)

compile(){
	flex lexic.lex
	yacc -d syntax.y
	gcc lex.yy.c y.tab.c effect_sup.c -o effect_reader
}

build(){
	if [[ ! -f effect_reader ]]; then 
		compile
	else
		for file in ${files[@]}; do
			if ( $(is_file_jounger $file effect_reader) ); then
				compile
				break;
			fi
		done
	fi
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