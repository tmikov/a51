#makefilefor djgpp make

ifdef _DEBUG
CCOPT=-D_DEBUG -DHEAP_DBG -g
else
CCOPT=-s -O1
endif

%.o: %.c
	gcc -c $(CCOPT) $<

link51.exe: link51.o err.o misc.o collect.o debug.o readrex.o process.o symbols.o segments.o output.o map.o
	gcc -o link51.exe $(CCOPT) link51.o err.o misc.o collect.o debug.o readrex.o process.o symbols.o segments.o output.o map.o

.PHONY: clean
clean:
	erase *.o
	erase *.exe

