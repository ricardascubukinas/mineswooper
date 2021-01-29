IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = board.h cell.h con_lib.h game_logic.h instance.h player.h save.h scoreboard.h utility.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o con_lib.o game_logic.o instance.o save.o scoreboard.o utility.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

game: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 