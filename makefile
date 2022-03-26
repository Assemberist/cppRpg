build:
	g++ *.cpp -o test -lncurses -lpanel -O2

debug:
	g++ *.cpp -o test -lncurses -lpanel -g
