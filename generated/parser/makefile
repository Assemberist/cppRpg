all:
	yacc -d syntax.y
	flex lexic.lex
	gcc lex.yy.c y.tab.c -o effect_reader
	cat test.txt | ./effect_reader

clean:
	rm *.c
	rm *.h
	rm effect_reader