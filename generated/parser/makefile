run:
	cat ../effects/CRASH_ATTACK.eff | ./effect_reader

build:
	flex lexic.lex
	yacc -d syntax.y
	gcc lex.yy.c y.tab.c effect_sup.c -o effect_reader


all: clean build run

clean:
	rm *.c
	rm *.h
	rm effect_reader