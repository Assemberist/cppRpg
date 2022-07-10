OBJS=obj/spell.o obj/text_field.o obj/effect_calc.o obj/object.o \
obj/classes.o obj/card.o obj/user_ifc.o obj/scene.o obj/main.o

debug: DBG :=-g
debug: $(OBJS)
	g++ obj/*.o -o test -lncurses -lpanel -g

build: DBG :=-O2
build: $(OBJS)
	g++ obj/*.o -o test -lncurses -lpanel -O2

obj/spell.o: spell.hpp
obj/text_field.o: text_field.hpp
obj/object.o: object.hpp object_defs.hpp obj/text_field.o obj/spell.o obj/effect_calc.o
obj/effect_calc.o:
obj/card.o: card.hpp object.hpp obj/object.o
obj/classes.o: classes.hpp obj/object.o
obj/user_ifc.o: user_ifc.hpp obj/card.o obj/classes.o
obj/scene.o: scene.hpp obj/user_ifc.o
obj/main.o: scene.hpp

$(OBJS): obj/%.o: %.cpp
	g++ -c $< -o $@ $(DBG)

clean:
	rm obj/*