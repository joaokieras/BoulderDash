// Projeto desenvolvido por João Pedro Kieras Oliveira
// GRR 20190379 Dinf - UFPR 
#ifndef __GAME__
#define __GAME__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "player.h"

#define SIZE_OBJS 30
#define MARGIN_TOP 30
#define WIDTH (40 * SIZE_OBJS)
#define HEIGHT ((22 * SIZE_OBJS) + MARGIN_TOP)
#define FRAMERATE 60.0
#define TEMPO_RESET 105
#define KEY_SEEN     1
#define KEY_RELEASED 2
#define SPAWN_X (3 * SIZE_OBJS)
#define SPAWN_Y ((2 * SIZE_OBJS) + MARGIN_TOP)
#define PATH_SPRITESHEET "resources/sprites/spritesheet.png"
#define PATH_MAP_1 "resources/maps/mapa1.txt"
#define PATH_SCORE "resources/scores/score.txt"
#define MIN_DIAMANTES 12
#define SIZE_ARQ_PONTOS 5

enum {INICIO, SERVINDO, JOGANDO, FIMPART, FIMJOGO} state;

void state_init();
void state_serve();
void state_play();
void state_end();
void state_close();

void inicia_allegro(bool teste, char *descricao);
void verifica_entradas(unsigned char *key, bool *done, bool redraw, player *jogador);
void draw(bool redraw, long frames);
void draw_instructions();
void draw_end_game();
void draw_inicial_menu();
void draw_hud();
#endif