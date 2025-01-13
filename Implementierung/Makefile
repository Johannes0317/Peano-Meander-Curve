CC = gcc
CFLAGS = -std=c11 -O3
LDFLAGS = -lm

.PHONY: all clean

all: main

main: main.c fast_power.c peano_meander.c svgwriter.c peano_meander_iterativ.c peano_meander_rec_opt.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f main