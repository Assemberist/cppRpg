enum_reader: lexic.lex syntax.y
	flex lexic.lex
	yacc -d syntax.y
	gcc *.c -o enum_reader

clean:
	rm *.c *.h enum_reader