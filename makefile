OBJS=obj/spell.o obj/text_field.o obj/state.o obj/object.o obj/classes.o obj/items.o \
obj/actions.o obj/effect_calc.o obj/card.o obj/battle_scene.o obj/scene.o obj/main.o \
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

obj/spell.o: object/spell.cpp object/spell.hpp
	g++ -c object/spell.cpp -o obj/spell.o $(DBG) $(trace) -Wall -Werror

obj/text_field.o: visual/curses/text_field.cpp visual/curses/text_field.hpp common/log.hpp
	g++ -c visual/curses/text_field.cpp -o obj/text_field.o $(DBG) $(trace) -Wall -Werror

obj/object_defs.o: object/object_defs.cpp object/object_defs.hpp
	g++ -c object/object_defs.cpp -o obj/object_defs.o $(DBG) $(trace) -Wall -Werror

obj/state.o: object/state.cpp object/state.hpp obj/object_defs.o obj/spell.o common/log.hpp
	g++ -c object/state.cpp -o obj/state.o $(DBG) $(trace) -Wall -Werror

obj/items.o: object/items.cpp object/items.hpp obj/state.o
	g++ -c object/items.cpp -o obj/items.o $(DBG) $(trace) -Wall -Werror

obj/object.o: object/object.cpp object/object.hpp obj/state.o common/drawable_object.hpp
	g++ -c object/object.cpp -o obj/object.o $(DBG) $(trace) -Wall -Werror

obj/card.o: object/object.hpp visual/curses/card.cpp visual/curses/card.cpp obj/object.o common/drawable_object.hpp
	g++ -c visual/curses/card.cpp -o obj/card.o $(DBG) $(trace) -Wall -Werror

obj/effect_calc.o: effect_calc.cpp obj/state.o
	g++ -c effect_calc.cpp -o obj/effect_calc.o $(DBG) $(trace) -Wall -Werror

obj/actions.o: actions.cpp actions.hpp obj/object.o
	g++ -c actions.cpp -o obj/actions.o $(DBG) $(trace) -Wall -Werror

obj/classes.o: classes.hpp classes.cpp obj/object.o
	g++ -c classes.cpp -o obj/classes.o $(DBG) $(trace) -Wall -Werror 

obj/user_ifc_lib.o: visual/curses/user_ifc_lib.cpp visual/curses/user_ifc_lib.hpp obj/card.o obj/classes.o obj/actions.o
	g++ -c visual/curses/user_ifc_lib.cpp -o obj/user_ifc_lib.o $(DBG) $(trace) -Wall -Werror

obj/battle_scene.o: visual/curses/battle_scene.cpp visual/curses/battle_scene.hpp obj/user_ifc_lib.o
	g++ -c visual/curses/battle_scene.cpp -o obj/battle_scene.o $(DBG) $(trace) -Wall -Werror

obj/scene.o: scene.cpp scene.hpp obj/battle_scene.o
	g++ -c scene.cpp -o obj/scene.o $(DBG) $(trace) -Wall -Werror

obj/main.o: main.cpp scene.hpp
	g++ -c main.cpp -o obj/main.o $(DBG) $(trace) -Wall -Werror

diag: $(DIAG_SRC)

diagram/%.png: diagram/%.uml
	plantuml $<

clean:
	rm obj/*.o