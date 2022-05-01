OBJS=obj/text_field.o obj/object.o obj/card.o obj/main.o obj/user_ifc.o

build: $(OBJS)
	g++ obj/*.o -o test -lncurses -lpanel -O2

debug: $(OBJS)
	g++ obj.o -o test -lncurses -lpanel -g

obj/text_field.o: text_field.cpp text_field.hpp
	g++ -c text_field.cpp -o obj/text_field.o -O2

obj/object.o: object.cpp object.hpp text_field.hpp object_defs.hpp
	g++ -c object.cpp -o obj/object.o -O2

obj/card.o: object.hpp card.cpp card.hpp
	g++ -c card.cpp -o obj/card.o -O2

obj/user_ifc.o: user_ifc.cpp user_ifc.hpp card.hpp classes.hpp object_defs.hpp
	g++ -c user_ifc.cpp -o obj/user_ifc.o -O2

obj/main.o: main.cpp card.hpp classes.hpp text_field.hpp user_ifc.hpp
	g++ -c main.cpp -o obj/main.o -O2
