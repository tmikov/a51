#BUILD=../b
A51=$(BUILD)/a51/a51
MLINK=$(BUILD)/mlink/mlink

AFLAGS=-l -c

%.obj : %.a51
	$(A51) $(AFLAGS) $<

simple.bin: simple.obj
	$(MLINK) $<

clean:
	rm -f *.bin *.obj *.lst

