EXE := macrobber-ng
CFLAGS := -O2 -Wall -g
LDLIBS := -lacl -lcap
DESTDIR := /usr/local
SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)
HDRS := $(wildcard *.h)

all:$(EXE)

$(EXE):$(OBJ)

$(OBJ): %.o: %.c $(HDRS)

clean:
	rm -f $(EXE) $(OBJ)

install:all
	install -Dm755 -t $(DESTDIR)/bin $(EXE)

.PHONY: all clean install
