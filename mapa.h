// Projeto desenvolvido por João Pedro Kieras Oliveira
// GRR 20190379 Dinf - UFPR 
#ifndef __MAPA__
#define __MAPA__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "game.h"
#include "sons.h"

#define METAL     0
#define TERRA     1
#define MURO      2
#define PEDRA     3
#define DIAMANTE  4
#define VAZIO     5
#define PLAYER    6
#define EXPLOSAO  7
#define EXPLOSAO2 8
#define EXPLOSAO3 9
#define SAIDA     10

struct rock{
  int x, y;
  int caindo;
};
typedef struct rock rock;

struct objetos{
  rock *rock;
  rock *diamond;
  ALLEGRO_BITMAP* terra;
  ALLEGRO_BITMAP* muro;
  ALLEGRO_BITMAP* metal;
  ALLEGRO_BITMAP* pedra;
  ALLEGRO_BITMAP* vazio;
  ALLEGRO_BITMAP* saida;
  ALLEGRO_BITMAP* diamante[8];
  ALLEGRO_BITMAP* explosao[4];
  int qntd_rocks, qntd_diamonds;
  int ciclos_diamante;
};
typedef struct objetos objetos;

void draw_map(int** mapa, audio* som, objetos* objetos_mapa, long frames);
int testa_game_over(int** mapa, audio* som, objetos* objetos_mapa, long frames, int tempo);
void testa_desmoronamento_pedra(int** mapa, audio* som, objetos* objetos_mapa, long frames);
void testa_desmoronamento_diamante(int** mapa, audio* som, objetos* objetos_mapa, long frames);
int** inicia_mapa(char* nome_mapa, objetos* obj);
void inicia_pedras_e_diamantes(int** mapa, objetos* obj);
objetos* inicia_objetos(ALLEGRO_BITMAP* sheet);
void busca_pedras_explosao(objetos* obj, int x, int y);
void busca_diamantes_explosao(objetos* obj, int x, int y);
void busca_e_destroi_pedra(objetos* obj, int x, int y);
void busca_e_destroi_diamante(objetos* obj, int x, int y);
void destroi_mapa(int** mapa);
void destroi_objetos(objetos* obj);
void verifica_rolamento_pedras(int** mapa, objetos* objetos_mapa, int pos_x, int pos_y, int i);
void verifica_rolamento_diamantes(int** mapa, objetos* objetos_mapa, int pos_x, int pos_y, int i);
void explode_em_volta(int** mapa, int pos_x, int pos_y);
#endif