all: wh

wh: main.o combat.o character.o dungeon.o gui.o
	gcc `pkg-config --cflags gtk+-3.0` main.o combat.o character.o dungeon.o gui.o -o wh `pkg-config --libs gtk+-3.0`

main.o: main.c adventures.h
	gcc `pkg-config --cflags gtk+-3.0` -ansi -g -c main.c `pkg-config --libs gtk+-3.0`

combat.o: combat.c adventures.h
	gcc `pkg-config --cflags gtk+-3.0` -ansi -g -c combat.c `pkg-config --libs gtk+-3.0`

character.o: character.c adventures.h
	gcc `pkg-config --cflags gtk+-3.0` -ansi -g -c character.c `pkg-config --libs gtk+-3.0`

dungeon.o: dungeon.c adventures.h
	gcc `pkg-config --cflags gtk+-3.0` -ansi -g -c dungeon.c `pkg-config --libs gtk+-3.0`

gui.o: gui.c adventures.h
	gcc `pkg-config --cflags gtk+-3.0` -ansi -g -c gui.c `pkg-config --libs gtk+-3.0`

clean:
	rm -rf *.o wh
