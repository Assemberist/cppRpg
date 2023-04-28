build: ../effect_reader
	echo Ok

../effect_reader: lexic.lex syntax.y effect_sup.c effect_sup.h
	flex lexic.lex
	yacc -d syntax.y
	gcc lex.yy.c y.tab.c effect_sup.c -o effect_reader
	mv effect_reader ..

all: clean build

clean:
	rm effect_reader lex.yy.c y.tab.h y.tab.c