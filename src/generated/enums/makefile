build: enum_reader $(patsubst sources/%.hpp,obj/%.o, $(wildcard sources/*.hpp))

obj/%.o: %.cpp
	gcc -c $< -o $@ -I sources

%.cpp: sources/%.hpp
	cat $< | ./enum_reader $(basename $@)

enum_reader: parser/enum_reader
	touch sources/*
	cp parser/enum_reader .

parser/enum_reader: FORCE
	cd parser && make

FORCE:

clean:
	cd parser && make clean
	rm enum_reader *.cpp