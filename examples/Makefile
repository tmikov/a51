#BUILD=../b
A51=$(BUILD)/a51/a51
LINK51=$(BUILD)/link51/link51

AFLAGS=-l -c
LFLAGS=-Th -m

%.obj : %.a51
	$(A51) $(AFLAGS) $<

all: simple.hex mod.hex

simple.hex: simple.obj
	$(LINK51) $(LFLAGS) -o$@ $<

mod.hex: mod1.obj mod2.obj
	$(LINK51) $(LFLAGS) -o$@ $^

clean:
	rm -f *.bin *.hex *.obj *.lst *.map

