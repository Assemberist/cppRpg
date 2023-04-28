PARTS := $(patsubst %.eff,%.part,$(wildcard *.eff))

build: parser ${PARTS}
	./compact.sh
	mv effect_calc.cpp ../..
	
%.part: %.eff effect_reader
	cat $< | ./effect_reader > $(basename $<).part

parser: FORCE
	cd parser && make

FORCE:

clean:
	rm *.part