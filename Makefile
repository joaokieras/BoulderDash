CC = gcc
EXEC = Boulder_Dash
MAIN = main.c 
ALLEGRO = $$(pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 --libs --cflags)
INCLUDE = game.c mapa.c player.c
CFLAG = Wall
OBJS = main.o game.o mapa.o player.o

all: hello

hello: $(OBJS)
	$(CC) -o $(EXEC) $(MAIN) $(INCLUDE) $(ALLEGRO)

main.o: main.c game.h
game.o: game.c game.h 
mapa.o: mapa.c mapa.h
player.o: player.c player.h

clean:
	-rm -f *~ *.o

purge:
	-rm -f $(OBJS) $(EXEC)