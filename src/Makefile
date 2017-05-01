
CC=gcc
CFLAGS= -fopenmp
LIBS = -lm
DEPS = radarHelpers.h
OBJ = radar.o radarHelpers.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

radar: $(OBJ)
	gcc -o $@ $^ $(LIBS) $(CFLAGS)

clean:
	rm -f *.o radar
