CC := gcc
LD := ld

CFLAGS := -Wall $(shell pkg-config --cflags --libs glib-2.0) $(shell pkg-config --libs --cflags smf)

SRC := $(shell find ./src -name '*.c')

all: build

.PHONY: build
build:
	$(CC) $(SRC) $(CFLAGS) -o fddmus

run: build
	./fddmus

clean:
	rm fddmus
