PARTS := $(patsubst %.eff,%.part,$(wildcard *.eff))

all: parser build

build: ${PARTS}
	./compact.sh

%.part: %.eff ./effect_reader
	cat $< | ./effect_reader > $(basename $<).part

parser: FORCE
	cd parser && make

FORCE:

clean:
	rm *.part