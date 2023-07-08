OBJS=obj/spell.o obj/text_field.o obj/state.o obj/object.o obj/classes.o obj/items.o \
obj/actions.o obj/effect_calc.o obj/card.o obj/user_ifc.o obj/scene.o obj/main.o \
obj/user_ifc_lib.o

DIAGS=$(wildcard diagram/*.uml)
DIAG_SRC=$(DIAGS:.uml=.png)

trace=$(trace1) $(trace2)

debug: DBG :=-g
debug: gen_effects $(OBJS)
	g++ obj/*.o -o test -lncurses -lpanel -g

build: DBG :=-O2
build: gen_effects $(OBJS)
	g++ obj/*.o -o test -lncurses -lpanel -O2

gen_effects: FORCE
	cd generated && make effects

FORCE:

obj/spell.o: spell.cpp spell.hpp
	g++ -c spell.cpp -o obj/spell.o $(DBG) $(trace) -Wall -Werror

obj/text_field.o: text_field.cpp text_field.hpp
	g++ -c text_field.cpp -o obj/text_field.o $(DBG) $(trace) -Wall -Werror

obj/object_defs.o: object_defs.cpp object_defs.hpp
	g++ -c object_defs.cpp -o obj/object_defs.o $(DBG) $(trace) -Wall -Werror

obj/state.o: state.cpp state.hpp obj/object_defs.o obj/spell.o
	g++ -c state.cpp -o obj/state.o $(DBG) $(trace) -Wall -Werror

obj/items.o: items.cpp items.hpp obj/state.o
	g++ -c items.cpp -o obj/items.o $(DBG) $(trace) -Wall -Werror

obj/object.o: object.cpp object.hpp obj/text_field.o obj/state.o drawable_object.hpp
	g++ -c object.cpp -o obj/object.o $(DBG) $(trace) -Wall -Werror

obj/card.o: object.hpp card.cpp card.hpp obj/object.o drawable_object.hpp
	g++ -c card.cpp -o obj/card.o $(DBG) $(trace) -Wall -Werror

obj/effect_calc.o: effect_calc.cpp obj/state.o
	g++ -c effect_calc.cpp -o obj/effect_calc.o $(DBG) $(trace) -Wall -Werror

obj/actions.o: actions.cpp actions.hpp obj/object.o
	g++ -c actions.cpp -o obj/actions.o $(DBG) $(trace) -Wall -Werror

obj/classes.o: classes.hpp classes.cpp obj/object.o
	g++ -c classes.cpp -o obj/classes.o $(DBG) $(trace) -Wall -Werror 

obj/user_ifc_lib.o: user_ifc_lib.cpp user_ifc_lib.hpp obj/card.o obj/classes.o obj/actions.o
	g++ -c user_ifc_lib.cpp -o obj/user_ifc_lib.o $(DBG) $(trace) -Wall -Werror

obj/user_ifc.o: user_ifc.cpp user_ifc.hpp obj/user_ifc_lib.o
	g++ -c user_ifc.cpp -o obj/user_ifc.o $(DBG) $(trace) -Wall -Werror

obj/scene.o: scene.cpp scene.hpp obj/user_ifc.o
	g++ -c scene.cpp -o obj/scene.o $(DBG) $(trace) -Wall -Werror

obj/main.o: main.cpp scene.hpp
	g++ -c main.cpp -o obj/main.o $(DBG) $(trace) -Wall -Werror

diag: $(DIAG_SRC)

diagram/%.png: diagram/%.uml
	plantuml $<

clean:
	rm obj/*.o