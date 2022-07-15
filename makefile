OBJS=obj/spell.o obj/text_field.o obj/object.o obj/classes.o \
obj/actions.o obj/effect_calc.o obj/card.o obj/user_ifc.o obj/scene.o obj/main.o

debug: DBG :=-g
debug: $(OBJS)
	g++ obj/*.o -o test -lncurses -lpanel -g

build: DBG :=-O2
build: $(OBJS)
	g++ obj/*.o -o test -lncurses -lpanel -O2

obj/spell.o: spell.cpp spell.hpp
	g++ -c spell.cpp -o obj/spell.o $(DBG)

obj/text_field.o: text_field.cpp text_field.hpp
	g++ -c text_field.cpp -o obj/text_field.o $(DBG)

obj/object.o: object.cpp object.hpp object_defs.hpp obj/text_field.o obj/spell.o
	g++ -c object.cpp -o obj/object.o $(DBG)

obj/card.o: object.hpp card.cpp card.hpp obj/object.o
	g++ -c card.cpp -o obj/card.o $(DBG)

obj/effect_calc.o: effect_calc.cpp effect_calc.hpp obj/object.o
	g++ -c effect_calc.cpp -o obj/effect_calc.o $(DBG)

obj/actions.o: actions.cpp actions.hpp obj/object.o
	g++ -c actions.cpp -o obj/actions.o $(DBG)

obj/classes.o: classes.hpp classes.cpp obj/object.o
	g++ -c classes.cpp -o obj/classes.o $(DBG)

obj/user_ifc.o: user_ifc.cpp user_ifc.hpp obj/card.o obj/classes.o obj/actions.o
	g++ -c user_ifc.cpp -o obj/user_ifc.o $(DBG)

obj/scene.o: scene.cpp scene.hpp obj/user_ifc.o
	g++ -c scene.cpp -o obj/scene.o $(DBG)

obj/main.o: main.cpp scene.hpp
	g++ -c main.cpp -o obj/main.o $(DBG)

clean:
	rm obj/*