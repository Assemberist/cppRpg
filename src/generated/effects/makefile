effect_calc.cpp: effect_reader $(patsubst sources/%.eff,%.part, $(wildcard sources/*.eff)) $(wildcard *.stdpart)
	cat head.stdpart *.part end.stdpart > effect_calc.cpp

%.part: sources/%.eff
	cat $< | ./effect_reader $(basename $@) > $@

effect_reader: parser/effect_reader
	touch sources/*
	cp parser/effect_reader .

parser/effect_reader: FORCE
	cd parser && make

FORCE:

clean:
	cd parser && make clean
	rm effect_reader *.part