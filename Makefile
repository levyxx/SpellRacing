game : game.c x11winlib.o kbhit.o draw.o object.o move.o save.o turtle.o
	gcc game.c x11winlib.o kbhit.o draw.o object.o move.o save.o turtle.o -lX11 -lm -lpthread -g -o game
x11winlib.o:x11winlib.c
	gcc x11winlib.c -c
kbhit.o:kbhit.c
	gcc kbhit.c -c
draw.o:draw.c
	gcc draw.c -c
object.o:object.c
	gcc object.c -c
move.o:move.c
	gcc move.c -c
save.o:save.c
	gcc save.c -c
turtle.o:turtle.c
	gcc turtle.c -c
all : game.c x11winlib.c kbhit.c draw.c object.c move.c save.c turtle.c
	gcc x11winlib.c -c
	gcc kbhit.c -c
	gcc draw.c -c
	gcc object.c -c
	gcc move.c -c
	gcc save.c -c
	gcc turtle.c -c
	gcc game.c x11winlib.o kbhit.o draw.o object.o move.o save.o turtle.o -lX11 -lm -lpthread -o game -g