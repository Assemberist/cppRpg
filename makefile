OBJS=obj/text_field.o obj/spell.o obj/object.o obj/classes.o \
obj/card.o obj/user_ifc.o obj/scene.o obj/main.o

debug: DBG :=-g
debug: $(OBJS)
	g++ obj/*.o -o test -lncurses -lpanel -g

build: DBG :=-O2
build: $(OBJS)
	g++ obj/*.o -o test -lncurses -lpanel -O2

obj/text_field.o: text_field.cpp text_field.hpp
	g++ -c text_field.cpp -o obj/text_field.o $(DBG)

obj/object.o: object.cpp object.hpp text_field.hpp object_defs.hpp
	g++ -c object.cpp -o obj/object.o $(DBG)

obj/card.o: object.hpp card.cpp card.hpp
	g++ -c card.cpp -o obj/card.o $(DBG)

obj/user_ifc.o: user_ifc.cpp user_ifc.hpp card.hpp classes.hpp object_defs.hpp
	g++ -c user_ifc.cpp -o obj/user_ifc.o $(DBG)

obj/main.o: main.cpp card.hpp classes.hpp text_field.hpp user_ifc.hpp
	g++ -c main.cpp -o obj/main.o $(DBG)

obj/spell.o: spell.cpp spell.hpp
	g++ -c spell.cpp -o obj/spell.o $(DBG)

obj/classes.o: classes.hpp classes.cpp 
	g++ -c classes.cpp -o obj/classes.o $(DBG)

obj/scene.o: scene.cpp scene.hpp
	g++ -c scene.cpp -o obj/scene.o $(DBG)

classes.hpp: spell.hpp object_defs.hpp

clean:
	rm obj/*