

CC=gcc
CFLAGS=-I.
DEPS = radarHelpers.h
OBJ = radar.o radarHelpers.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

radar: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o radar
