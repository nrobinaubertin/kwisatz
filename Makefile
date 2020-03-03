.POSIX:
.SUFFIXES:

CC=clang
CFLAGS=-Wall -Wno-unused-result -std=c11 -pedantic -Wno-return-stack-address -I./include
LDFLAGS=
LDLIBS=-pthread

DFLAGS=-ggdb
RFLAGS=-DNDEBUG -O3

src = $(wildcard src/*.c) $(wildcard src/structs/*.c)
obj = $(src:.c=.o)

release: CFLAGS+=$(RFLAGS)
release: chess

debug: CFLAGS+=$(DFLAGS)
debug: chess

chess: $(obj)
	$(CC) $(LDLIBS) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f $(obj) chess

.PHONY: clean all debug release
