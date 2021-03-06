OBJS=obj/spell.o obj/text_field.o obj/state.o obj/object.o obj/classes.o \
obj/actions.o obj/effect_calc.o obj/card.o obj/user_ifc.o obj/scene.o obj/main.o

trace=$(trace1) $(trace2)

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

obj/state.o: state.cpp state.hpp object_defs.hpp obj/spell.o
	g++ -c state.cpp -o obj/state.o $(DBG) $(trace)

obj/object.o: object.cpp object.hpp obj/text_field.o obj/state.o
	g++ -c object.cpp -o obj/object.o $(DBG) $(trace)

obj/card.o: object.hpp card.cpp card.hpp obj/object.o
	g++ -c card.cpp -o obj/card.o $(DBG) $(trace)

obj/effect_calc.o: effect_calc.cpp obj/state.o
	g++ -c effect_calc.cpp -o obj/effect_calc.o $(DBG) $(trace)

obj/actions.o: actions.cpp actions.hpp obj/object.o
	g++ -c actions.cpp -o obj/actions.o $(DBG) $(trace)

obj/classes.o: classes.hpp classes.cpp obj/object.o
	g++ -c classes.cpp -o obj/classes.o $(DBG) 

obj/user_ifc.o: user_ifc.cpp user_ifc.hpp obj/card.o obj/classes.o obj/actions.o
	g++ -c user_ifc.cpp -o obj/user_ifc.o $(DBG) $(trace)

obj/scene.o: scene.cpp scene.hpp obj/user_ifc.o
	g++ -c scene.cpp -o obj/scene.o $(DBG) $(trace)

obj/main.o: main.cpp scene.hpp
	g++ -c main.cpp -o obj/main.o $(DBG) $(trace)

clean:
	rm obj/*