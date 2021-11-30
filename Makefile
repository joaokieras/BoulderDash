# Projeto desenvolvido por João Pedro Kieras Oliveira
# GRR 20190379 Dinf - UFPR 
CC = gcc
EXEC = Boulder_Dash
MAIN = main.c 
ALLEGRO = $$(pkg-config --libs allegro-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 --libs --cflags)
INCLUDE = game.c mapa.c player.c sprites.c sons.c score.c
CFLAG = Wall
OBJS = main.o game.o mapa.o player.o sprites.o sons.o score.o

all: hello

hello: $(OBJS)
	$(CC) -o $(EXEC) $(MAIN) $(INCLUDE) $(ALLEGRO)

main.o: main.c game.h
game.o: game.c game.h 
mapa.o: mapa.c mapa.h
sons.o: sons.c sons.h
score.o: score.c score.h
player.o: player.c player.h
sprites.o: sprites.c sprites.h

clean:
	-rm -f *~ *.o

purge:
	-rm -f $(OBJS) $(EXEC)